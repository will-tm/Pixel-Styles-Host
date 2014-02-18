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
mode_fading::mode_fading(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable)
		: mode_interface(pWidth, pHeight, pName, pAudioAvailable)
{
	mCurrentColor = hue_to_rgb(random(360));

	// Settings;
	if (mAudioAvailable)
		mSettings.add("Sound Reactive", "Audio", "True", 0.0, 1.0, ihmCheckbox);
	mSettings.add("Fading", "Color", "True", 0.0f, 1.0f, ihmCheckbox);
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
	bool fadingColors = mSettings["Fading"]->get_value<bool>();
	
	mBitmap->fill(mCurrentColor);

	if (fadingColors)
	{
		mCurrentColor = inc_hue_of_color(mCurrentColor, 1);
	}
}

void mode_fading::beat_detected()
{
	mCurrentColor = inc_hue_of_color(mCurrentColor, 80);
}
