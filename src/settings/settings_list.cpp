/*
 * settings_list.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "settings_list.h"

void list_setting_did_change_callback(void* parent, setting *pSetting)
{
	((settings_list*) parent)->setting_did_change(pSetting);
}

/*
 * constructor
 *
 */
settings_list::settings_list()
{
	mIniParser = NULL;
}

/*
 * destructor
 *
 */
settings_list::~settings_list()
{
	mSettingsMap.clear();
	mSettingsList.clear();
	if (mIniParser != NULL)
		delete mIniParser;
}

/*
 * public functions
 *
 */
setting *settings_list::add(string pCaption, string pSection, string pValue, float pMinValue, float pMaxValue, ihm_type pIhmType)
{
	setting *aSetting = new setting(pCaption, pSection, pValue, pMinValue, pMaxValue, pIhmType);
	aSetting->register_callback(this, list_setting_did_change_callback);
	mSettingsMap[pCaption] = aSetting;
	mSettingsList.push_back(pCaption);
	return aSetting;
}

setting *settings_list::operator[](const string &pName)
{
	return mSettingsMap[pName];
}

setting *settings_list::operator[](const int &pIndex)
{
	return mSettingsMap[mSettingsList[pIndex]];
}

size_t settings_list::size()
{
	return mSettingsList.size();
}

void settings_list::set_ini_path(string pPath)
{
	mIniParser = new ini_parser(pPath);
	
	if (size() > 0)
	{
		for (size_t i = 0; i < size(); i++)
		{
			setting *aSetting = mSettingsMap[mSettingsList[i]];
			string key = aSetting->caption;
			replace_all(key, " ", "_");
			string value = mIniParser->get<string>("SETTINGS", key, aSetting->get_value<string>());
			aSetting->set_value(value);
		}
	}
}

void settings_list::setting_did_change(setting *pSetting)
{
	string key = pSetting->caption;
	replace_all(key, " ", "_");
	mIniParser->set("SETTINGS", key, pSetting->get_value<string>());
}
