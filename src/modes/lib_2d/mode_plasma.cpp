/*
 * mode_plasma.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_plasma.h"

/*
 * public library interface
 *
 */
extern "C" mode_interface* create_mode(size_t pWidth, size_t pHeight, bool pAudioAvailable, vector<size_t> pSegments)
{
	return new mode_plasma(pWidth, pHeight, "Plasma", pAudioAvailable, pSegments);
}

extern "C" void destroy_mode(mode_interface* object)
{
	delete object;
}

/*
 * constructor
 *
 */
mode_plasma::mode_plasma(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable, vector<size_t> pSegments)
		: mode_interface(pWidth, pHeight, pName, pAudioAvailable, pSegments)
{
	mGridLayer = mBitmap->add_layer();
	
	mHueMatrix.resize(mWidth);
	for (auto &column : mHueMatrix)
	{
		column.resize(mHeight);
		for (auto &item : column)
			item = rand() % 720;
	}
	
	// Settings
	if (mAudioAvailable)
		mSettings.add("Sound Reactive", "Audio", "1", 0.0, 1.0, ihmCheckbox);
	mSettings.add("Grid", "Overlay", "1", 0.0, 1.0, ihmCheckbox);
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
 * private functions
 *
 */
void mode_plasma::setup_grid_layer_with_alpha(uint8_t alpha)
{
	rgb_color girdColor = ColorBlack;
	girdColor.A = alpha;
	
	for (size_t y = 0; y < mHeight; y++)
	{
		for (size_t x = 0; x < mWidth; x++)
		{
			mGridLayer->set_pixel(x, y, ((x % 2) == (y % 2)) ? girdColor : ColorClear);
		}
	}
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
	bool showGridOverlay = mSettings["Grid"]->get_value<bool>();

	// Grid overlay
	setup_grid_layer_with_alpha(showGridOverlay ? 120 : 0);
	
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
			hsvColor.A = 255;
			
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
