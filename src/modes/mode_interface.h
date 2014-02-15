/*
 * mode_interface.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

using namespace std;

#include <mutex>
#include <algorithm>
#include <sys/time.h>

#include "hsv_helper.h"
#include "bitmap.h"
#include "settings_list.h"
#include "ini_parser.h"
#include "strings-helper.h"
#include "time-helper.h"
#include "global_event_loop.h"
#include "config.h"

#define random(x)	rand()%x
/*
 * public types
 *
 */
typedef enum
{
	uiSpectrum, uiSettings
} ui_type;

typedef enum
{
	touchUp = 0, touchDown, touchMove
} touch_type;

/*
 * public class
 *
 */
class mode_interface
{
protected:
	size_t mWidth;
	size_t mHeight;
	string mName;
	string mNameFile;
	string mIniFilePath;
	float mAudioLevel;
	float mSpeed;
	bool mSoundReactive;
	bool mAudioAvailable;
	long mLastTime;
	ini_parser *mIniFile;
	ui_type mUI;
	bitmap *mBitmap;

	float randomf(float min, float max)
	{
		float random_ratio = ((float) rand()) / (float) RAND_MAX;
		float range = max - min;
		return (random_ratio * range) + min;
	}
public:
	settings_list mSettings;

	mode_interface(size_t pWidth, size_t pHeight, string pName,
			bool pAudioAvailable)
	{
		mWidth = pWidth;
		mHeight = pHeight;
		mAudioLevel = 0.0f;
		mSpeed = 0.0f;
		mLastTime = get_tick_us();
		mSoundReactive = false;
		mAudioAvailable = pAudioAvailable;
		mName = pName;
		mNameFile = pName;
		replace_all(mNameFile, " ", "_");
		transform(mNameFile.begin(), mNameFile.end(), mNameFile.begin(),
				::tolower);
		mIniFilePath = (string) CONFIGURATION_DIRECTORY + "settings_"
				+ mNameFile + ".cfg";
		mIniFile = NULL;
		mUI = uiSettings;
		mBitmap = new bitmap(mWidth, mHeight);
	}
	
	virtual ~mode_interface()
	{
		delete mIniFile;
		delete mBitmap;
	}
	virtual void paint() = 0;
	virtual void beat_detected()
	{
	}
	virtual void initialize(vector<rgb_color> pStaticColors)
	{
	}
	virtual void touch(vector<rgb_color> pStaticColors, touch_type pTouchType)
	{
	}
	virtual bool needs_audio_level()
	{
		return false;
	}
	virtual bool needs_audio_fft()
	{
		return false;
	}
	void set_audio_level(float pAudioLevel)
	{
		mAudioLevel = pAudioLevel;
	}
	string name()
	{
		return mName;
	}
	ui_type ui()
	{
		return mUI;
	}
	bitmap *get_bitmap()
	{
		return mBitmap;
	}
	size_t get_width()
	{
		return mWidth;
	}
	size_t get_height()
	{
		return mHeight;
	}
};
