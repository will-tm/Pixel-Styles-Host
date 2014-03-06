/*
 * mode_rainbow.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_rainbow.h"

/*
 * public library interface
 *
 */
extern "C" mode_interface* create_mode(size_t pWidth, size_t pHeight, bool pAudioAvailable, vector<size_t> pSegments)
{
  return new mode_rainbow(pWidth, pHeight, "Rainbow", pAudioAvailable, pSegments);
}

extern "C" void destroy_mode(mode_interface* object)
{
  delete object;
}

/*
 * constructor
 *
 */
mode_rainbow::mode_rainbow(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable, vector<size_t> pSegments) : mode_interface(pWidth, pHeight, pName, pAudioAvailable, pSegments)
{
	mRotationPtr = 0.0f;

	// Settings
	if(mAudioAvailable) mSettings.add("Sound Reactive", 		"Audio", 	"True", 0.0,  1.0,   ihmCheckbox);
	mSettings.add("Fading Speed",			"Colors",  	"10",   1.0,  100.0, ihmTrackbar);
	mSettings.set_ini_path(mIniFilePath);
}

/*
 * destructor
 *
 */
mode_rainbow::~mode_rainbow()
{

}

/*
 * public functions
 *
 */
void mode_rainbow::paint()
{
	mSpeed = mSettings["Fading Speed"]->get_value<float>();

	for(size_t i = 0; i < mSize; i++)
	{
		float currentHue = 359.0f / mSize * i + mRotationPtr;
		while (currentHue > 359.0f) currentHue = currentHue - 359.0f;

		hsv_color rotationColor;
		rotationColor.H = currentHue;
		rotationColor.S = 255;
		rotationColor.V = 255;

		mBitmap->set_hsv_pixel(i,0,rotationColor);
	}

	mRotationPtr = mRotationPtr - (0.015f * mSpeed);
	while (mRotationPtr < 0.0f)	mRotationPtr = mRotationPtr + 359.0f;
}

void mode_rainbow::beat_detected()
{
	if(!mAudioAvailable) return;

	mSoundReactive = mSettings["Sound Reactive"]->get_value<bool>();
	if(mSoundReactive)
	{
		mRotationPtr = mRotationPtr - 20.0f;
	}
}

void mode_rainbow::initialize(vector <rgb_color> pStaticColors)
{
	mRotationStartColor = rgb_to_hsv(pStaticColors[0]);
}
