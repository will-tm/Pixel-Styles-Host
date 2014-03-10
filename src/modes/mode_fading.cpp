/*
 * mode_fading.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_fading.h"

/*
 * constructor
 *
 */
mode_fading::mode_fading(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable, vector<size_t> pSegments)
		: mode_interface(pWidth, pHeight, pName, pAudioAvailable, pSegments)
{
	mRotationPtr = 0.0f;

	// Settings
	if(mAudioAvailable) 
		mSettings.add("Sound Reactive", 		"Audio", 	"True", 0.0,  1.0,   ihmCheckbox);
	mSettings.add("Fading Speed",			"Colors",  	"10",   1.0,  100.0, ihmTrackbar);
	if (mHeight == 1)
		mSettings.add("Link All", 				"Colors", 	"True", 0.0,  1.0, 	 ihmCheckbox);

	mSettings.set_ini_path(mIniFilePath);
}

/*
 * destructor
 *
 */
mode_fading::~mode_fading()
{

}

/*
 * public functions
 *
 */
void mode_fading::paint()
{
	size_t sectorOffset = 0;
	size_t sectorIndex = 0;
	hsv_color rotationColor;

	mSpeed = mSettings["Fading Speed"]->get_value<float>();

	mLinkedSectors = (mHeight > 1)? true : mSettings["Link All"]->get_value<bool>();
	if(mLinkedSectors)
	{
		rotationColor.H = (int)mRotationPtr;
		rotationColor.S = mRotationStartColor.S;
		rotationColor.V = mRotationStartColor.V;

		mBitmap->fill(hsv_to_rgb(rotationColor));
	}
	else
	{
		for(size_t i = 0; i < mWidth; i++)
		{
			if((i-sectorOffset) >= mSegments[sectorIndex])
			{
				sectorOffset += mSegments[sectorIndex];
				sectorIndex++;
			}
			rotationColor.H = (int)mRotationPtr + sectorIndex * (360 / mSegments.size());
			rotationColor.S = mRotationStartColor.S;
			rotationColor.V = mRotationStartColor.V;
			mBitmap->set_pixel(i, 0, hsv_to_rgb(rotationColor));
		}
	}

	mRotationPtr = mRotationPtr - (0.006 * mSpeed);
	while (mRotationPtr < 0.0) mRotationPtr = mRotationPtr + 359.0;
}

void mode_fading::beat_detected()
{
	if(!mAudioAvailable) return;

	mSoundReactive = mSettings["Sound Reactive"]->get_value<bool>();
	if(mSoundReactive)
	{
		mRotationPtr = mRotationPtr - 20.0f;
	}
}

void mode_fading::initialize(vector <rgb_color> pStaticColors)
{
	mRotationStartColor = rgb_to_hsv(pStaticColors[0]);
	mRotationPtr = mRotationStartColor.H;
}
