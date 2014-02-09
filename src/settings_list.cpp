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
/* Name : settings_list.cpp																	*/
/* Date : Sep 05 2012																		*/
/* Author : William Markezana																*/
/********************************************************************************************/

/********************************************************************************************/
/* INCLUDES																					*/
/********************************************************************************************/
#include "settings_list.h"

/********************************************************************************************/
/* CALLBACK																					*/
/********************************************************************************************/
void list_setting_did_change_callback(void* parent, setting *pSetting)
{
	((settings_list*)parent)->setting_did_change(pSetting);
}

/********************************************************************************************/
/* CONSTRUCTOR																				*/
/********************************************************************************************/
settings_list::settings_list()
{
	mIniParser = NULL;
}

/********************************************************************************************/
/* DESTRUCTOR																				*/
/********************************************************************************************/
settings_list::~settings_list()
{
	mSettingsMap.clear();
	mSettingsList.clear();
	if(mIniParser != NULL)
		delete mIniParser;
}

/********************************************************************************************/
/* PUBLIC FUNCTIONS																			*/
/********************************************************************************************/
setting *settings_list::add(string _caption, string _section, string _value, float _minValue, float _maxValue, ihm_type _ihmType)
{
	setting *aSetting = new setting(_caption, _section, _value, _minValue, _maxValue, _ihmType);
	aSetting->register_callback(this, list_setting_did_change_callback);
	mSettingsMap[_caption] = aSetting;
	mSettingsList.push_back(_caption);
	return aSetting;
}

setting *settings_list::operator[] (const string &__Name)
{
	return mSettingsMap[__Name];
}

setting *settings_list::operator[] (const int &__Index)
{
	return mSettingsMap[mSettingsList[__Index]];
}

size_t settings_list::size()
{
	return mSettingsList.size();
}

void settings_list::set_ini_path(string pPath)
{
	mIniParser = new ini_parser(pPath);

	if(size() > 0)
	{
		for(size_t i = 0; i < size(); i++)
		{
			setting *aSetting = mSettingsMap[mSettingsList[i]];
			string key = aSetting->caption;
			replace_all(key, " ", "_");
			if(mIniParser->get<double>("SETTINGS", key, -1.0) != -1.0)
			{
				string value = mIniParser->get<string>("SETTINGS", key, "1");
				aSetting->set_value(value);
			}
		}
	}
}

void settings_list::setting_did_change(setting *pSetting)
{
	//printf("setting_did_change %s %s\n", pSetting->caption.c_str(), pSetting->get_value<string>().c_str());
	string key = pSetting->caption;
	replace_all(key, " ", "_");
	mIniParser->set("SETTINGS", key, pSetting->get_value<string>());
}

/********************************************************************************************/
/* END OF FILE																				*/
/********************************************************************************************/
