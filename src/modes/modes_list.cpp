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
/* Name : modes_list.cpp																	*/
/* Date : Jul 24 2012																		*/
/* Author : William Markezana																*/
/********************************************************************************************/

/********************************************************************************************/
/* INCLUDES																					*/
/********************************************************************************************/
#include "modes_list.h"

#include <muduo/base/Logging.h>

/********************************************************************************************/
/* CONSTRUCTOR																				*/
/********************************************************************************************/
modes_list::modes_list()
{

}

/********************************************************************************************/
/* DESTRUCTOR																				*/
/********************************************************************************************/
modes_list::~modes_list()
{
	mModesMap.clear();
	mModesList.clear();
}

/********************************************************************************************/
/* PUBLIC FUNCTIONS																			*/
/********************************************************************************************/
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

/********************************************************************************************/
/* END OF FILE																				*/
/********************************************************************************************/
