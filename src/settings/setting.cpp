/*
 * setting.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "setting.h"

/*
 * constructor
 *
 */
setting::setting(string pCaption, string pSection, string pValue, float pMinValue, float pMaxValue, ihm_type pIhmType)
{
	mValue = pValue;
	caption = pCaption;
	section = pSection;
	kind = pIhmType;
	min_value = pMinValue;
	max_value = pMaxValue;
	mParent = NULL;
	mSettingDidChangeCallback = NULL;
}

/*
 * destructor
 *
 */
setting::~setting()
{
	
}
