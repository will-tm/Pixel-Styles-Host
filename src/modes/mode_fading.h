/*
 * mode_fading.h
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
class mode_fading: public mode_interface
{
private:
	rgb_color mCurrentColor;
public:
	mode_fading(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable);
	~mode_fading();

	virtual void paint();
	virtual void beat_detected();
};
