/*
 * mode_spectrum_3d.h
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
class mode_spectrum_3d: public mode_interface
{
private:
	hsv_color mLastStartColor;
	hsv_color mLastEndColor;
public:
	mode_spectrum_3d(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable);
	~mode_spectrum_3d();

	virtual void paint();
	virtual bool needs_audio_fft() { return true; };
};
