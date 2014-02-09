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
/* Name : settings_list.h																	*/
/* Date : Sep 05 2012																		*/
/* Author : William Markezana																*/
/********************************************************************************************/

#ifndef __SETTINGS_LIST_H__
#define __SETTINGS_LIST_H__

using namespace std;

/********************************************************************************************/
/* INCLUDES																					*/
/********************************************************************************************/
#include "setting.h"
#include "ini_parser.h"
#include <map>

/********************************************************************************************/
/* CLASSES																					*/
/********************************************************************************************/
class settings_list
{
private:
	ini_parser* mIniParser;
	map<string,setting*> mSettingsMap;
	vector<string> mSettingsList;
public:
	settings_list();
	~settings_list();

	setting *add(string _caption, string _section, string _value, float _minValue, float _maxValue, ihm_type _ihmType);
	setting *operator[] (const string &__Name);
	setting *operator[] (const int &__Index);
	size_t size();
	void set_ini_path(string pPath);
	void setting_did_change(setting *pSetting);
};

/********************************************************************************************/
/* END OF FILE																				*/
/********************************************************************************************/
#endif
