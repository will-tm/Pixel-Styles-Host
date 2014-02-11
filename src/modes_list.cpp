/*
 * modes_list.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "modes_list.h"

#include <muduo/base/Logging.h>

/*
 * constructor
 *
 */
modes_list::modes_list()
{

}

/*
 * destructor
 *
 */
modes_list::~modes_list()
{
	mModesMap.clear();
	mModesList.clear();
}

/*
 * public functions
 *
 */
void modes_list::add(string pCaption, mode_interface *pMode)
{
	mModesMap[pCaption] = pMode;
	mModesList.push_back(pCaption);

	LOG_INFO << pCaption << " mode initialized";
}

mode_interface *modes_list::operator[] (const string &pName)
{
	return mModesMap[pName];
}

mode_interface *modes_list::operator[] (const int &pIndex)
{
	return mModesMap[mModesList[pIndex]];
}

size_t modes_list::size()
{
	return mModesMap.size();
}

int modes_list::operator[] (const mode_interface *mode)
{
	int result = 0;
	for(map<string,mode_interface*>::iterator iter = mModesMap.begin(); iter != mModesMap.end(); iter++ )
	{
		if((*iter).second == mode) return result;
		result++;
	}
	return -1;
}
