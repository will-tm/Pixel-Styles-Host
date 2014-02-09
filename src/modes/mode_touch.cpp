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
/* Name : mode_touch.cpp																	*/
/* Date : Jul 24 2013																		*/
/* Author : William Markezana																*/
/********************************************************************************************/

/********************************************************************************************/
/* INCLUDES																					*/
/********************************************************************************************/
#include "mode_touch.h"

/********************************************************************************************/
/* CONSTRUCTOR																				*/
/********************************************************************************************/
mode_touch::mode_touch(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable) : mode_interface(pWidth, pHeight, pName, pAudioAvailable)
{
	mIniFile = new ini_parser(mIniFilePath);
	mUI = uiSpectrum;
}

/********************************************************************************************/
/* DESTRUCTOR																				*/
/********************************************************************************************/
mode_touch::~mode_touch()
{
	delete mIniFile;
}

/********************************************************************************************/
/* PUBLIC FUNCTIONS																			*/
/********************************************************************************************/
void mode_touch::paint()
{
	mColorsMutex.lock();

	rgb_color currentColor = mStaticColors[0];
	mBitmap->fill(currentColor);

	mColorsMutex.unlock();
}

void mode_touch::initialize(vector <rgb_color> pStaticColors)
{
	mColorsMutex.lock();
	mStaticColors = pStaticColors;
	mColorsMutex.unlock();
}

void mode_touch::touch(vector <rgb_color> pStaticColors, touch_type pTouchType)
{
	mColorsMutex.lock();
	mStaticColors = pStaticColors;
	mColorsMutex.unlock();
}

/********************************************************************************************/
/* END OF FILE																				*/
/********************************************************************************************/
