/*
 * mode_spectrum.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

using namespace std;

#include "mode_interface.h"

/*
 * public types
 *
 */
typedef struct
{
	int position;
	uint32_t tick;
	bool atMax;
} peak_t;

/*
 * public class
 *
 */
class mode_spectrum: public mode_interface
{
private:
	hsv_color mLastStartColor;
	hsv_color mLastEndColor;
	vector<peak_t> mPeaks;
public:
	mode_spectrum(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable);
	~mode_spectrum();

	virtual void paint();
	virtual bool needs_audio_fft() { return true; };
};
