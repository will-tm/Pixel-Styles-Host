/*
 * settings_list.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#ifndef __SETTINGS_LIST_H__
#define __SETTINGS_LIST_H__

using namespace std;

#include "setting.h"
#include "ini_parser.h"
#include <map>

/*
 * public class
 *
 */
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

#endif
