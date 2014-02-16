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

static BOOL CALLBACK duff_recording(HRECORD handle, const void *buffer, DWORD length, void *user);
static void CALLBACK get_beat_pos(DWORD chan, double beatpos, void *user);

/*
 * constructor
 *
 */
modes_controller::modes_controller(size_t pWidth, size_t pHeight)
{
	mIniFile = new ini_parser((string) CONFIGURATION_DIRECTORY + "config.cfg");
	mWidth = pWidth;
	mHeight = pHeight;
	mAudioLevel = 0.0f;
	mAudioLevelRatio = log10f(1.0f / 65536.0f);
	mAudioAvailable = false;
	mBypassBASS = false;
	mLastUdpFrameTick = 0;
	mSensitivity = 1.0;
	
	mSpectrum.resize(mWidth);
	mScope.resize(mWidth);
	mPows.resize(mWidth);
	
	for (size_t x = 0; x < mWidth; x++)
		mPows[x] = pow(2.0f, (float) x * 9.0f / ((float) mWidth - 1.0f));
	
	mUdpServer = new udp_server(get_global_event_loop(), 56617);
	mUdpServer->register_read_callback(bind(&modes_controller::handle_receive, this, _1, _2));
	
	if (BASS_RecordInit(-1))
	{
		mRecordChannel = BASS_RecordStart(48000, 1, 0, &duff_recording, this);
		BASS_FX_BPM_BeatCallbackSet(mRecordChannel, &get_beat_pos, this);
		mAudioAvailable = true;
	}
	else
	{
		LOG_WARN<< "Can't initialize audio device";
	}

	mModesList.add("OFF", new mode_off(mWidth, mHeight, "OFF", mAudioAvailable));
	mModesList.add("Touch", new mode_touch(mWidth, mHeight, "Touch", mAudioAvailable));
	mModesList.add("Plasma", new mode_plasma(mWidth, mHeight, "Plasma", mAudioAvailable));
	mModesList.add("Balls", new mode_balls(mWidth, mHeight, "Balls", mAudioAvailable));
	mModesList.add("UDP Streamer", new mode_udp_streamer(mWidth, mHeight, "UDP Streamer", mAudioAvailable));
	
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
static BOOL CALLBACK duff_recording(HRECORD handle, const void *buffer, DWORD length, void *user)
{
	return TRUE;
}

static void CALLBACK get_beat_pos(DWORD chan, double beatpos, void *user)
{
	((modes_controller*) user)->beat_detected();
	LOG_INFO << "Beat detected!";
}

void modes_controller::handle_receive(uint8_t *data, size_t length)
{
	if (length != 1024 * sizeof(float))
		return;
	
	memcpy(mFftData, data, length);
	
	process_fft_buffer_1024(mFftData);
	mBypassBASS = true;
	mLastUdpFrameTick = get_tick_us();
}

/*
 * private functions
 *
 */
void modes_controller::audio_tasks()
{
	if (get_tick_us() - mLastUdpFrameTick > 2000000)
		mBypassBASS = false;
	
	if (!mBypassBASS && mAudioAvailable && mActiveMode->needs_audio_fft())
	{
		BASS_ChannelGetData(mRecordChannel, (void*) &mIntFftData[0],
		BASS_DATA_FFT2048);
		for (int i = 0; i < 1024; i++)
			mFftData[i] = (float) mIntFftData[i] / 16777216.0f * 128.0f;
		
		process_fft_buffer_1024(mFftData);
	}
	
	if (mAudioAvailable && mActiveMode->needs_audio_level())
	{
		DWORD level, both;
		WORD left, right;
		level = BASS_ChannelGetLevel(mRecordChannel);
		left = LOWORD(level);
		right = HIWORD(level);
		both = left + right;
		//mAudioLevel = log10f((1.0f+(float)both)/65536.0f) / mAudioLevelRatio;
		mAudioLevel = (float) both / 65536.0f;
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
	mActiveMode->get_bitmap()->render();
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
	generics.push_back(Pair("active_mode", mActiveMode->name()));
	generics.push_back(Pair("mac_address", mTcpServer->mac_address()));
	generics.push_back(Pair("width", (int) mWidth));
	generics.push_back(Pair("height", (int) mHeight));
	generics.push_back(Pair("audio_available", mAudioAvailable));
	json.push_back(generics);
	
	Object modes;
	Array modes_array;
	for (size_t i = 0; i < mModesList.size(); i++)
	{
		mode_interface *mode = mModesList[i];
		Object mode_json;
		mode_json.push_back(Pair("name", mode->name()));
		mode_json.push_back(Pair("ui", mode->ui()));
		mode_json.push_back(Pair("pixels", mode->get_bitmap()->to_string()));
		Array mode_settings_array;
		for (size_t j = 0; j < mode->mSettings.size(); j++)
		{
			Object setting_json;
			setting *aSetting = mode->mSettings[j];
			
			setting_json.push_back(Pair("caption", aSetting->caption));
			setting_json.push_back(Pair("kind", aSetting->kind));
			setting_json.push_back(Pair("minValue", aSetting->min_value));
			setting_json.push_back(Pair("maxValue", aSetting->max_value));
			setting_json.push_back(Pair("section", aSetting->section));
			setting_json.push_back(Pair("value", aSetting->get_value<string>()));
			
			mode_settings_array.push_back(setting_json);
		}
		mode_json.push_back(Pair("settings", mode_settings_array));
		modes_array.push_back(mode_json);
	}
	modes.push_back(Pair("modes", modes_array));
	json.push_back(modes);
	
	return write(json, raw_utf8);
}

string modes_controller::json_success()
{
	Object json;
	json.push_back(Pair("success", 1));
	return write(json, raw_utf8);
}

string modes_controller::json_error()
{
	Object json;
	json.push_back(Pair("success", 0));
	return write(json, raw_utf8);
}

string modes_controller::bitmap_to_json(bitmap *pBitmap)
{
	Object bitmapJSON;
	bitmapJSON.push_back(Pair("mac_address", mTcpServer->mac_address()));
	bitmapJSON.push_back(Pair("width", pBitmap->width));
	bitmapJSON.push_back(Pair("height", pBitmap->height));
	bitmapJSON.push_back(Pair("pixels", pBitmap->to_string()));
	return write(bitmapJSON, raw_utf8);
}
string modes_controller::active_mode_bitmap_to_json()
{
	return bitmap_to_json(active_mode()->get_bitmap());
}

void modes_controller::initialize(vector<rgb_color> pStaticColors)
{
	for (size_t i = 0; i < mModesList.size(); i++)
	{
		mModesList[i]->initialize(pStaticColors);
		for (int j = 0; j < 32; j++)
			mModesList[i]->paint();
	}
}

void modes_controller::process_fft_buffer_1024(float *fftdata)
{
	size_t y;
	int size;
	float sc;
	int b0 = 0;
	
	for (size_t x = 0; x < mWidth; x++)
	{
		float peak = 0;
		int b1 = mPows[x];
		if (b1 > 1023)
			b1 = 1023;
		if (b1 <= b0)
			b1 = b0 + 1;
		sc = 10 + b1 - b0;
		size = b1 - b0;
		peak = 0;
		for (int i = 0; i < size; i++)
		{
			float b2 = (float) fftdata[1 + b0 + i] * mSensitivity;
			peak = peak + b2;
		}
		b0 = b1;
		y = (size_t) ((sqrt(peak / log10(sc)) * (float) mHeight - 1.0f));
		if (y > mHeight)
			y = mHeight;
		mSpectrum[x] = y;
	}
}
