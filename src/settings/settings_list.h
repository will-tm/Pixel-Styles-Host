/*
 * settings_list.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

using namespace std;

#include "setting.h"
#include "ini_parser.h"
#include <map>
#include <vector>
#include <algorithm>

/*
 * public class
 *
 */
class settings_list
{
private:
	ini_parser* mIniParser;
	map<string, setting*> mSettingsMap;
	vector<string> mSettingsList;
	
	void setting_did_change(setting *pSetting);
public:
	settings_list();
	~settings_list();

	setting *add(string pCaption, string pSection, string pValue, float pMinValue, float pMaxValue, ihm_type pIhmType);
	setting *operator[](const string &pName);
	setting *operator[](const int &pIndex);
	size_t size();
	void set_ini_path(string pPath);
};
