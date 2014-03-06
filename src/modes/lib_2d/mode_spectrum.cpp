/*
 * mode_spectrum.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_spectrum.h"

#define PEAK_TOP_DELAY					(200000) // us
#define PEAK_FALL_DELAY					(60000) // us

/*
 * public library interface
 *
 */
extern "C" mode_interface* create_mode(size_t pWidth, size_t pHeight, bool pAudioAvailable, vector<size_t> pSegments)
{
  return new mode_spectrum(pWidth, pHeight, "Spectrum", pAudioAvailable, pSegments);
}

extern "C" void destroy_mode(mode_interface* object)
{
  delete object;
}

/*
 * constructor
 *
 */
mode_spectrum::mode_spectrum(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable, vector<size_t> pSegments)
		: mode_interface(pWidth, pHeight, pName, pAudioAvailable, pSegments)
{
	mPeaks.resize(mWidth);
	for (peak_t &peak : mPeaks)
	{
		peak.position = -1;
		peak.tick = get_tick_us();
		peak.atMax = false;
	}
	
	// Settings
	mSettings.add("Enabled", "Blur", "1", 0.0f, 1.0f, ihmCheckbox);
	mSettings.add("Length", "Blur", "89.0", 0.0f, 100.0f, ihmLogTrackbar);
	mSettings.add("Start", "Color", rgb_to_string(ColorGreen), 0.0f, 4294967295.0f, ihmColorSelector);
	mSettings.add("End", "Color", rgb_to_string(ColorBlue), 0.0f, 4294967295.0f, ihmColorSelector);
	mSettings.add("Gradient", "Color", "1", 0.0f, 0.0, ihmCheckbox);
	mSettings.add("Fading", "Color", "1", 0.0f, 1.0f, ihmCheckbox);
	mSettings.add("Max peaks", "Audio", "1", 0.0f, 1.0f, ihmCheckbox);
	mSettings.set_ini_path(mIniFilePath);

	mLastStartColor = int_to_hsv(mSettings["Start"]->get_value<uint32_t>());
	mLastEndColor = int_to_hsv(mSettings["End"]->get_value<uint32_t>());
}

/*
 * destructor
 *
 */
mode_spectrum::~mode_spectrum()
{
	
}

/*
 * public functions
 *
 */
void mode_spectrum::paint()
{
	bool blurEffect = mSettings["Enabled"]->get_value<bool>();
	int blurLength = (int)mSettings["Length"]->max_value - mSettings["Length"]->get_value<int>();
	bool fadingColors = mSettings["Fading"]->get_value<bool>();
	bool gradientColors = mSettings["Gradient"]->get_value<bool>();
	hsv_color startColor = int_to_hsv(mSettings["Start"]->get_value<uint32_t>());
	hsv_color endColor = int_to_hsv(mSettings["End"]->get_value<uint32_t>());
	bool maxPeaks = mSettings["Max peaks"]->get_value<bool>();

	float hueInc;
	if (mLastStartColor.H > mLastEndColor.H)
		hueInc = (float)((mLastEndColor.H + 360 - mLastStartColor.H) % 360) / (float)mWidth;
	else
		hueInc = (float)((mLastEndColor.H - mLastStartColor.H) % 360) / (float)mWidth;

	hsv_color currentColor = mLastStartColor;

	if (!blurEffect)
	{
		mBitmap->clear();
	}
	else
	{
		for (size_t y = 0; y < mHeight; y++)
		{
			for (size_t x = 0; x < mWidth; x++)
			{
				rgb_color color = mBitmap->get_pixel(x, y);
				if (color.R >= blurLength)
					color.R -= blurLength;
				else color.R = 0;
				if (color.G >= blurLength)
					color.G -= blurLength;
				else color.G = 0;
				if (color.B >= blurLength)
					color.B -= blurLength;
				else color.B = 0;
				mBitmap->set_pixel(x, y, color);
			}
		}
	}

	for (size_t x = 0; x < mWidth; x++)
	{
		if (mSpectrum[x] > 0)
		{
			for (size_t y = 0; y < mSpectrum[x]; y++)
			{
				mBitmap->set_hsv_pixel(x,mHeight-1-y, currentColor);
			}

			if (gradientColors)
			{
				currentColor.H += (int)(hueInc);
			}
		}

		if (maxPeaks)
		{
			if ((int)mSpectrum[x] > mPeaks[x].position)
			{
				mPeaks[x].position = mSpectrum[x]-1;
				mPeaks[x].tick = get_tick_us();
				mPeaks[x].atMax = true;
			}
			if ((get_tick_us() - mPeaks[x].tick > PEAK_TOP_DELAY) && mPeaks[x].atMax)
			{
				mPeaks[x].atMax = false;
				mPeaks[x].tick = get_tick_us();
			}
			if ((get_tick_us() - mPeaks[x].tick > PEAK_FALL_DELAY) && !mPeaks[x].atMax)
			{
				if (mPeaks[x].position > -1) mPeaks[x].position--;
				mPeaks[x].tick = get_tick_us();
			}
			mBitmap->set_pixel(x,mHeight-1-mPeaks[x].position, ColorWhite);
		}
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
