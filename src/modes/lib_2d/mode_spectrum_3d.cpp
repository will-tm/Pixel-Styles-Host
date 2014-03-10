/*
 * mode_spectrum_3d.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_spectrum_3d.h"

/*
 * public library interface
 *
 */
extern "C" mode_interface* create_mode(size_t pWidth, size_t pHeight, bool pAudioAvailable, vector<size_t> pSegments)
{
	return new mode_spectrum_3d(pWidth, pHeight, "Spectrum 3D", pAudioAvailable, pSegments);
}

extern "C" void destroy_mode(mode_interface* object)
{
	delete object;
}

/*
 * constructor
 *
 */
mode_spectrum_3d::mode_spectrum_3d(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable, vector<size_t> pSegments)
		: mode_interface(pWidth, pHeight, pName, pAudioAvailable, pSegments)
{
	// Settings
	//mSettings.add("Speed", "Scroll", "10.0", 0.0f, 100.0f, ihmLogTrackbar);
	mSettings.add("Start", "Color", rgb_to_string(ColorGreen), 0.0f, 4294967295.0f, ihmColorSelector);
	mSettings.add("End", "Color", rgb_to_string(ColorBlue), 0.0f, 4294967295.0f, ihmColorSelector);
	mSettings.add("Fading", "Color", "0", 0.0f, 1.0f, ihmCheckbox);
	mSettings.set_ini_path(mIniFilePath);

	mLastStartColor = int_to_hsv(mSettings["Start"]->get_value<uint32_t>());
	mLastEndColor = int_to_hsv(mSettings["End"]->get_value<uint32_t>());
}

/*
 * destructor
 *
 */
mode_spectrum_3d::~mode_spectrum_3d()
{
	
}

/*
 * public functions
 *
 */
void mode_spectrum_3d::paint()
{
	//int speed = mSettings["Speed"]->get_value<int>();
	bool fadingColors = mSettings["Fading"]->get_value<bool>();
	hsv_color startColor = int_to_hsv(mSettings["Start"]->get_value<uint32_t>());
	hsv_color endColor = int_to_hsv(mSettings["End"]->get_value<uint32_t>());

	float hueInc;
	if (mLastStartColor.H > mLastEndColor.H)
		hueInc = (float)((mLastEndColor.H + 360 - mLastStartColor.H) % 360) / (float)mWidth;
	else
		hueInc = (float)((mLastEndColor.H - mLastStartColor.H) % 360) / (float)mWidth;

	hsv_color currentColor = mLastStartColor;

	for(int x = 0; x < mWidth; x++)
	{
		for(int y = 1; y < mHeight; y++)
		{
			mBitmap->set_hsv_pixel(x,y-1, mBitmap->get_hsv_pixel(x,y));
		}
	}

	for(int x = 0; x < mWidth; x++)
	{
		mBitmap->set_pixel(x, mHeight-1, hue_to_rgb(mLastStartColor.H + mSpectrum[x]*(int)hueInc));
	}

	if (fadingColors)
	{
		if (++mLastStartColor.H >= 360) mLastStartColor.H = 0;
		if (++mLastEndColor.H >= 360) mLastEndColor.H = 0;
	}
	else
	{
		mLastStartColor = startColor;
		mLastEndColor = endColor;
	}
}
