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
/* Name : modes_list.h																		*/
/* Date : Jul 24 2012																		*/
/* Author : William Markezana																*/
/********************************************************************************************/

#ifndef __MODES_LIST_H__
#define __MODES_LIST_H__

using namespace std;

/********************************************************************************************/
/* INCLUDES																					*/
/********************************************************************************************/
#include "mode_interface.h"
#include <string>
#include <vector>
#include <map>

/********************************************************************************************/
/* CLASSES																					*/
/********************************************************************************************/
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

/********************************************************************************************/
/* END OF FILE																				*/
/********************************************************************************************/
#endif
