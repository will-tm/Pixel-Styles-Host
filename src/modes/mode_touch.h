/*
 * mode_touch.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#ifndef __MODE_TOUCH_H__
#define __MODE_TOUCH_H__

using namespace std;

#include "mode_interface.h"

/*
 * public class
 *
 */
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

#endif
