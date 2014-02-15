/*
 * mode_plasma.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_plasma.h"

/*
 * constructor
 *
 */
mode_plasma::mode_plasma(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable)
		: mode_interface(pWidth, pHeight, pName, pAudioAvailable)
{
	mHueMatrix.resize(mWidth);
	for (auto &column : mHueMatrix)
	{
		column.resize(mHeight);
		for (auto &item : column)
			item = rand() % 720;
	}
	
	// ====================================== SETTINGS =======================================
	if (mAudioAvailable)
		mSettings.add("Sound Reactive", "Audio", "True", 0.0, 1.0, ihmCheckbox);
	mSettings.set_ini_path(mIniFilePath);
}

/*
 * destructor
 *
 */
mode_plasma::~mode_plasma()
{
	
}

/*
 * public functions
 *
 */
void mode_plasma::paint()
{
#define COEF_FILTER			(4)
	
	int left, right, above, below;
	hsv_color hsvColor;
	int currentHue;
	float averageHue;
	uint16_t uint16AverageHue;
	bool soundReactive = mSettings["Sound Reactive"]->get_value<bool>();
	
	for (size_t j = 0; j < mHeight; j++)
	{
		for (size_t i = 0; i < mWidth; i++)
		{
			left = i - 1;
			right = i + 1;
			above = j - 1;
			below = j + 1;
			
			if (left < 0)
				left = mWidth - 1;
			if (left > (int) mWidth - 1)
				left = 0;
			if (right < 0)
				right = mWidth - 1;
			if (right > (int) mWidth - 1)
				right = 0;
			if (above < 0)
				above = mHeight - 1;
			if (above > (int) mHeight - 1)
				above = 0;
			if (below < 0)
				below = mHeight - 1;
			if (below > (int) mHeight - 1)
				below = 0;
			
			hsvColor = mBitmap->get_hsv_pixel(i, j);
			
			currentHue = mHueMatrix[i][j];
			
			averageHue = ((float) (mHueMatrix[left][j]) + (float) (mHueMatrix[right][j]) + (float) (mHueMatrix[i][above])
					+ (float) (mHueMatrix[i][below])) / 4.0f;
			
			if (averageHue != currentHue)
			{
				uint16AverageHue = (uint16_t) ceil((currentHue * (COEF_FILTER - 1) + averageHue + 6) / COEF_FILTER);
				mHueMatrix[i][j] = uint16AverageHue;
			}
			
			if (!soundReactive)
			{
				if (rand() % 100000 > 99945)
				{
					mHueMatrix[i][j] = 0;
					mHueMatrix[right][j] = 0;
					mHueMatrix[i][below] = 0;
				}
			}
			
			hsvColor.H = (mHueMatrix[i][j] % 720) / 2;
			hsvColor.S = 255;
			hsvColor.V = 255;
			
			mBitmap->set_hsv_pixel(i, j, hsvColor);
		}
	}
}

void mode_plasma::beat_detected()
{
	for (size_t i = 0; i < (size_t) (rand() % 4); i++)
	{
		mHueMatrix[rand() % mWidth][rand() % mHeight] = 0;
	}
}
