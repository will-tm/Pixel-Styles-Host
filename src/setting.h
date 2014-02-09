/********************************************************************************************/
/* Copyright (c) 2012 RGB Styles															*/
/********************************************************************************************/

/********************************************************************************************/
/* This file is the confidential trade secret information and/or proprietary information	*/
/* of RGB Styles, Inc. Code or other information in this program also may be confidential	*/
/* and/or proprietary to RGB Styles, Inc.													*/
/* All rights reserved.																		*/
/********************************************************************************************/

/********************************************************************************************/
/* Name : setting.h																			*/
/* Date : Sep 03 2012																		*/
/* Author : William Markezana																*/
/********************************************************************************************/

#ifndef __SETTING_H__
#define __SETTING_H__

using namespace std;

/********************************************************************************************/
/* INCLUDES																					*/
/********************************************************************************************/
#include <vector>
#include <sstream>
#include <string>
#include <math.h>
#include <mutex>
#include "strings-helper.h"
#include "hsv_helper.h"


class setting;
/********************************************************************************************/
/* TYPEDEFS																					*/
/********************************************************************************************/
typedef enum
{
    ihmSpinEdit,
    ihmSpinEditFloat,
    ihmCheckbox,
    ihmSegmentedControl,
    ihmTrackbar
}ihm_type;

typedef void (setting_did_change_callback)(void* parent, setting *pSetting);

/********************************************************************************************/
/* CLASSES																					*/
/********************************************************************************************/
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

	template <typename T> T get_value()
	{
		mMutex.lock();
		T result = from_string<T>(mValue);
		mMutex.unlock();
		return result;
	}
	template <typename T> void set_value(const T& v)
	{
		mMutex.lock();
		mValue = to_string<T>(v);
		mMutex.unlock();

		if(mSettingDidChangeCallback != NULL)
		{
			mSettingDidChangeCallback(mParent, this);
		}
	}
};

/********************************************************************************************/
/* END OF FILE																				*/
/********************************************************************************************/
#endif
