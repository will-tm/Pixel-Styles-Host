/*
 * modes_controller.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "modes_controller.h"

#include <muduo/base/Logging.h>
#include <zlib.h>
#include <iostream>

using namespace json_spirit;

static BOOL CALLBACK DuffRecording(HRECORD handle, const void *buffer, DWORD length, void *user);
static void CALLBACK GetBeatPos_Callback(DWORD chan, double beatpos, void *user);

/*
 * constructor
 *
 */
modes_controller::modes_controller(size_t pWidth, size_t pHeight)
{
	mIniFile = new ini_parser("/etc/pixel_styles/config.cfg");
	mWidth = pWidth;
	mHeight = pHeight;
	mAudioLevel = 0.0f;
	mAudioLevelRatio = log10f(1.0f/65536.0f);
	mAudioAvailable = false;

	if (BASS_RecordInit(-1))
	{
		mRecordChannel = BASS_RecordStart(48000,1,0,&DuffRecording, this);
		BASS_FX_BPM_BeatCallbackSet(mRecordChannel, &GetBeatPos_Callback, this);
		mAudioAvailable = true;
	}
	else
	{
		LOG_WARN << "Can't initialize audio device";
	}

	mModesList.add("OFF", 		new mode_off		(mWidth, mHeight, "OFF",		mAudioAvailable));
	mModesList.add("Touch", 	new mode_touch		(mWidth, mHeight, "Touch",		mAudioAvailable));
	mModesList.add("Plasma", 	new mode_plasma		(mWidth, mHeight, "Plasma",		mAudioAvailable));

	mActiveMode = mModesList[0];
	LOG_INFO << "active mode is now '" << mActiveMode->name() << "'";

	delete mIniFile;

	LOG_INFO << "modes_controller initialized";
}

/*
 * destructor
 *
 */
modes_controller::~modes_controller()
{

}

/*
 * private callbacks
 *
 */
static BOOL CALLBACK DuffRecording(HRECORD handle, const void *buffer, DWORD length, void *user)
{
	return TRUE;
}

static void CALLBACK GetBeatPos_Callback(DWORD chan, double beatpos, void *user)
{
	((modes_controller*)user)->beat_detected();

	LOG_INFO << "Beat detected!";
}

/*
 * private functions
 *
 */
void modes_controller::audio_tasks()
{
	if(mAudioAvailable && mActiveMode->needs_audio_fft())
	{
		BASS_ChannelGetData(mRecordChannel, (void*)&mFftData[0], BASS_DATA_FFT2048);
	}

	if(mAudioAvailable && mActiveMode->needs_audio_level())
	{
		DWORD level, both;
		WORD left, right;
		level = BASS_ChannelGetLevel(mRecordChannel);
		left = LOWORD(level);
		right = HIWORD(level);
		both = left + right;
		//mAudioLevel = log10f((1.0f+(float)both)/65536.0f) / mAudioLevelRatio;
		mAudioLevel = (float)both/65536.0f;
		mActiveMode->set_audio_level(mAudioLevel);
	}
}

/*
 * public functions
 *
 */
void modes_controller::beat_detected()
{
	mActiveMode->beat_detected();
}

void modes_controller::paint()
{
	audio_tasks();
	mActiveMode->paint();
}

bitmap *modes_controller::active_mode_bitmap()
{
	return mActiveMode->get_bitmap();
}

void modes_controller::set_active_mode_name(const string pName)
{
	mActiveMode = mModesList[pName];

	LOG_INFO << "active mode is now '" << mActiveMode->name() << "'";
}

string modes_controller::to_json()
{
	Array json;

	Object generics;
	generics.push_back(Pair("active_mode",mActiveMode->name()));
	generics.push_back(Pair("mac_address",mTcpServer->mac_address()));
	generics.push_back(Pair("width",(int)mWidth));
	generics.push_back(Pair("height",(int)mHeight));
	generics.push_back(Pair("audio_available",mAudioAvailable));
	json.push_back(generics);

	Object modes;
	Array modes_array;
	for(size_t i = 0; i < mModesList.size(); i++)
	{
		mode_interface *mode = mModesList[i];
		Object mode_json;
		mode_json.push_back(Pair("name",mode->name()));
		mode_json.push_back(Pair("ui",mode->ui()));
		mode_json.push_back(Pair("pixels",mode->get_bitmap()->to_string()));
		Array mode_settings_array;
		for(size_t j = 0; j < mode->mSettings.size(); j++)
		{
			Object setting_json;
			setting *aSetting = mode->mSettings[j];

			setting_json.push_back(Pair("caption",aSetting->caption));
			setting_json.push_back(Pair("kind",aSetting->kind));
			setting_json.push_back(Pair("minValue",aSetting->min_value));
			setting_json.push_back(Pair("maxValue",aSetting->max_value));
			setting_json.push_back(Pair("section",aSetting->section));
			setting_json.push_back(Pair("value",aSetting->get_value<string>()));

			mode_settings_array.push_back(setting_json);
		}
		mode_json.push_back(Pair("settings",mode_settings_array));
		modes_array.push_back(mode_json);
	}
	modes.push_back(Pair("modes",modes_array));
	json.push_back(modes);

	return write(json, raw_utf8);
}

string modes_controller::json_success()
{
	Object json;
	json.push_back(Pair("success",1));
	return write(json, raw_utf8);
}

string modes_controller::json_error()
{
	Object json;
	json.push_back(Pair("success",0));
	return write(json, raw_utf8);
}

string modes_controller::bitmap_to_json(bitmap *pBitmap)
{
	Object bitmapJSON;
	bitmapJSON.push_back(Pair("mac_address",mTcpServer->mac_address()));
	bitmapJSON.push_back(Pair("width",pBitmap->width));
	bitmapJSON.push_back(Pair("height",pBitmap->height));
	bitmapJSON.push_back(Pair("pixels",pBitmap->to_string()));
	return write(bitmapJSON, raw_utf8);
}
string modes_controller::active_mode_bitmap_to_json()
{
	return bitmap_to_json(active_mode()->get_bitmap());
}

void modes_controller::initialize(vector <rgb_color> pStaticColors)
{
	for (size_t i = 0; i < mModesList.size(); i++)
	{
		mModesList[i]->initialize(pStaticColors);
		for (int j = 0; j < 32; j++)
			mModesList[i]->paint();
	}
}
