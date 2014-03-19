/*
 * mode_touch.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

using namespace std;

#include "mode_interface.h"

/*
 * public class
 *
 */
class mode_touch: public mode_interface
{
private:
	mutex mColorsMutex;
	vector<rgb_color> mStaticColors;
public:
	mode_touch(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable, vector<size_t> pSegments);
	~mode_touch();

	virtual void paint();
	virtual void initialize(vector<rgb_color> pStaticColors);
	virtual void touch(vector<rgb_color> pStaticColors, touch_type pTouchType);
};
