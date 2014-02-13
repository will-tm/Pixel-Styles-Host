/*
 * modes_list.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

using namespace std;

#include "mode_interface.h"
#include <string>
#include <vector>
#include <map>

/*
 * public class
 *
 */
class modes_list
{
private:
	map<string,mode_interface*> mModesMap;
	vector<string> mModesList;
public:
	modes_list();
	~modes_list();

	void add(string _caption, mode_interface *mode);
	mode_interface *operator[] (const string &name);
	mode_interface *operator[] (const int &index);
	int operator[] (const mode_interface *mode);
	size_t size();
};
