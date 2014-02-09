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
/* Name : mode_touch.h																		*/
/* Date : Jul 24 2013																		*/
/* Author : William Markezana																*/
/********************************************************************************************/

#ifndef __MODE_TOUCH_H__
#define __MODE_TOUCH_H__

using namespace std;

/********************************************************************************************/
/* INCLUDES																					*/
/********************************************************************************************/
#include "mode_interface.h"

/********************************************************************************************/
/* TYPEDEFS																					*/
/********************************************************************************************/

/********************************************************************************************/
/* CLASSES																					*/
/********************************************************************************************/
class mode_touch : public mode_interface
{
private:
	mutex mColorsMutex;
	vector <rgb_color> mStaticColors;
public:
	mode_touch(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable);
	~mode_touch();

	virtual void paint();
	virtual void initialize(vector <rgb_color> pStaticColors);
	virtual void touch(vector <rgb_color> pStaticColors, touch_type pTouchType);
};

/********************************************************************************************/
/* END OF FILE																				*/
/********************************************************************************************/
#endif
