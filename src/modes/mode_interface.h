/********************************************************************************************/
/* Copyright (c) 2013 RGB Styles															*/
/********************************************************************************************/

/********************************************************************************************/
/* This file is the confidential trade secret information and/or proprietary information	*/
/* of RGB Styles, Inc. Code or other information in this program also may be confidential	*/
/* and/or proprietary to RGB Styles, Inc.													*/
/* All rights reserved.																		*/
/********************************************************************************************/

/********************************************************************************************/
/* Name : mode_interface.h																	*/
/* Date : Jul 24 2013																		*/
/* Author : William Markezana																*/
/********************************************************************************************/

#ifndef __MODE_INTERFACE_H__
#define __MODE_INTERFACE_H__

using namespace std;

/********************************************************************************************/
/* INCLUDES																					*/
/********************************************************************************************/
#include <mutex>
#include <algorithm>
#include "hsv_helper.h"
#include "bitmap.h"
#include "settings_list.h"
#include "ini_parser.h"
#include "strings-helper.h"
#include "time-helper.h"

/********************************************************************************************/
/* TYPEDEFS																					*/
/********************************************************************************************/
typedef enum
{
    uiSpectrum,
    uiSettings
}ui_type;

typedef enum
{
	touchUp = 0,
	touchDown,
	touchMove
}touch_type;

/********************************************************************************************/
/* CLASSES																					*/
/********************************************************************************************/
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
		float random = ((float) rand()) / (float) RAND_MAX;
		float range = max - min;
		return (random*range) + min;
	}
public:
	settings_list mSettings;

	mode_interface(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable)
	{
		mWidth = pWidth;
		mHeight = pHeight;
		mAudioLevel = 0.0f;
		mSpeed = 0.0f;
		mLastTime = getCurrentTime();
		mSoundReactive = false;
		mAudioAvailable = pAudioAvailable;
		mName = pName;
		mNameFile = pName;
		replace_all(mNameFile," ","_");
		transform(mNameFile.begin(), mNameFile.end(), mNameFile.begin(), ::tolower);
		mIniFilePath = "/etc/pixel_styles/settings_"+mNameFile+".cfg";
		mIniFile = NULL;
		mUI = uiSettings;
		mBitmap = new bitmap(mWidth, mHeight);
	}

	virtual ~mode_interface() { delete mIniFile; delete mBitmap; }
	virtual void paint() = 0;
	virtual void beat_detected() {}
	virtual void initialize(vector <rgb_color> pStaticColors) {}
	virtual void touch(vector <rgb_color> pStaticColors, touch_type pTouchType) {}
	virtual bool needs_audio_level() { return false; }
	virtual bool needs_audio_fft() { return false; }
	void set_audio_level(float pAudioLevel) { mAudioLevel = pAudioLevel; }
	string name() { return mName; }
	ui_type ui() { return mUI; }
	bitmap *get_bitmap() { return mBitmap; }
};

/********************************************************************************************/
/* END OF FILE																				*/
/********************************************************************************************/
#endif
