/*
 * mode_cca.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_cca.h"

#define STATES_COUNT  9

/*
 * public library interface
 *
 */
extern "C" mode_interface* create_mode(size_t pWidth, size_t pHeight, bool pAudioAvailable, vector<size_t> pSegments)
{
	return new mode_cca(pWidth, pHeight, "CCA", pAudioAvailable, pSegments);
}

extern "C" void destroy_mode(mode_interface* object)
{
	delete object;
}

/*
 * constructor
 *
 */
mode_cca::mode_cca(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable, vector<size_t> pSegments)
		: mode_interface(pWidth, pHeight, pName, pAudioAvailable, pSegments)
{
	mGridLayer = mBitmap->add_layer();
	
	mData.resize(mWidth);
	for (auto &column : mData)
	{
		column.resize(mHeight);
		for (auto &item : column)
			item = rand() % STATES_COUNT;
	}

	mDataBuffer.resize(mWidth);
	for (auto &column : mDataBuffer)
	{
		column.resize(mHeight);
		for (auto &item : column)
			item = rand() % STATES_COUNT;
	}
	
	mIterations = 0;
	mEmptyFrames = 0;

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
mode_cca::~mode_cca()
{
	
}

/*
 * private functions
 *
 */
void mode_cca::setup_grid_layer_with_alpha(uint8_t alpha)
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
void mode_cca::paint()
{
	if (++mEmptyFrames < 5)
		return;

	mEmptyFrames = 0;
	int i, j, nextStep, left, right, above, below, aWidth, aHeight;
	hsv_color hsvColor;
	hsvColor.S = 255;
	hsvColor.V = 255;

	bool soundReactive = mSettings["Sound Reactive"]->get_value<bool>();
	bool showGridOverlay = mSettings["Grid"]->get_value<bool>();

	// Grid overlay
	setup_grid_layer_with_alpha(showGridOverlay ? 120 : 0);

	mIterations++;
	for (i = 0; i < mData.size(); i++)
	{
		aWidth = mData.size();
		aHeight = mData[i].size();
		for (j = 0; j < mData[i].size(); j++)
		{
			left = i - 1;
			right = i + 1;
			above = j - 1;
			below = j + 1;
			
			if (left < 0)
				left = aWidth - 1;
			if (left > aWidth - 1)
				left = 0;
			if (right < 0)
				right = aWidth - 1;
			if (right > aWidth - 1)
				right = 0;
			if (above < 0)
				above = aHeight - 1;
			if (above > aHeight - 1)
				above = 0;
			if (below < 0)
				below = aHeight - 1;
			if (below > aHeight - 1)
				below = 0;
			
			if (mData[i][j] < (STATES_COUNT - 1))
			{
				nextStep = mData[i][j] + 1;
			}
			else nextStep = 0;
			
			if (mData[left][above] == nextStep)
				mDataBuffer[i][j] = nextStep;
			if (mData[left][j] == nextStep)
				mDataBuffer[i][j] = nextStep;
			if (mData[left][below] == nextStep)
				mDataBuffer[i][j] = nextStep;
			if (mData[i][above] == nextStep)
				mDataBuffer[i][j] = nextStep;
			if (mData[i][below] == nextStep)
				mDataBuffer[i][j] = nextStep;
			if (mData[right][above] == nextStep)
				mDataBuffer[i][j] = nextStep;
			if (mData[right][j] == nextStep)
				mDataBuffer[i][j] = nextStep;
			if (mData[right][below] == nextStep)
				mDataBuffer[i][j] = nextStep;
		}
	}

	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			mData[x][y] = mDataBuffer[x][y];
		}
	}
	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			hsvColor.H = int((mData[x][y] * 250) / (STATES_COUNT - 1));
			mBitmap->set_hsv_pixel(x, y, hsvColor);
		}
	}

	if (!soundReactive)
	{
		if (!(mIterations % (random(25) + 15)))
		{
			for (int y = 0; y < mHeight; y++)
			{
				for (int x = 0; x < mWidth; x++)
				{
					mData[x][y] = random(STATES_COUNT);
				}
			}
		}
	}
}

void mode_cca::beat_detected()
{
	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			mData[x][y] = random(STATES_COUNT);
		}
	}
}
