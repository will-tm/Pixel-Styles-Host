/*
 * mode_touch.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_touch.h"

/*
 * constructor
 *
 */
mode_touch::mode_touch(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable, vector<size_t> pSegments)
		: mode_interface(pWidth, pHeight, pName, pAudioAvailable, pSegments)
{
	mUI = uiSpectrum;
    
    // Settings
    mSettings.add("Color", "Invader", rgb_to_string(ColorGreen), 0.0f, 4294967295.0f, ihmColorSelector);
    mSettings.set_ini_path(mIniFilePath);
}

/*
 * destructor
 *
 */
mode_touch::~mode_touch()
{
	
}

/*
 * public functions
 *
 */
void mode_touch::paint()
{
	mColorsMutex.lock();

	if (mHeight == 1)
	{
		size_t step = mWidth / mStaticColors.size();
		size_t nextStep = step;
		size_t staticPtr = 0;

		for(size_t i = 0; i < mWidth; i++)
		{
			rgb_color currentColor = mStaticColors[staticPtr];
			mBitmap->set_pixel(i,0,currentColor);

			if(i >= nextStep)
			{
				nextStep += step;
				staticPtr++;
			}
		}
	}
	else
	{
		rgb_color currentColor = mStaticColors[0];
		mBitmap->fill(currentColor);
	}

	mColorsMutex.unlock();
}

void mode_touch::initialize(vector<rgb_color> pStaticColors)
{
	mColorsMutex.lock();
	mStaticColors = pStaticColors;
	mColorsMutex.unlock();
}

void mode_touch::touch(vector<rgb_color> pStaticColors, touch_type pTouchType)
{
	mColorsMutex.lock();
	mStaticColors = pStaticColors;
	mColorsMutex.unlock();
}
