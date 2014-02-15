/*
 * setting.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

using namespace std;

#include <vector>
#include <sstream>
#include <string>
#include <math.h>
#include <mutex>
#include "strings-helper.h"
#include "hsv_helper.h"

/*
 * public types
 *
 */
class setting;

typedef enum
{
	ihmSpinEdit, ihmSpinEditFloat, ihmCheckbox, ihmSegmentedControl, ihmTrackbar, ihmLogTrackbar, ihmButton
} ihm_type;

typedef void (setting_did_change_callback)(void* parent, setting *pSetting);

/*
 * public class
 *
 */
class setting
{
private:
	mutex mMutex;
	string mValue;
	setting_did_change_callback *mSettingDidChangeCallback;
	void *mParent;
public:
	string caption;
	string section;
	float min_value;
	float max_value;
	ihm_type kind;

	setting(string pCaption, string pSection, string pValue, float pMinValue, float pMaxValue, ihm_type pIhmType);
	~setting();

	void register_callback(void *pParent, setting_did_change_callback *pSettingDidChangeCallback)
	{
		mParent = pParent;
		mSettingDidChangeCallback = pSettingDidChangeCallback;
	}
	
	template<typename T> T get_value()
	{
		mMutex.lock();
		T result = from_string<T>(mValue);
		mMutex.unlock();
		return result;
	}
	template<typename T> void set_value(const T& v)
	{
		mMutex.lock();
		mValue = to_string<T>(v);
		mMutex.unlock();
		
		if (mSettingDidChangeCallback != NULL)
		{
			mSettingDidChangeCallback(mParent, this);
		}
	}
};
