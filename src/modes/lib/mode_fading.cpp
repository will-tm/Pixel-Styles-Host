/*
 * mode_fading.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_fading.h"

/*
 * public library interface
 *
 */
extern "C" mode_interface* create_mode(size_t pWidth, size_t pHeight, bool pAudioAvailable)
{
  return new mode_fading(pWidth, pHeight, "Fading", pAudioAvailable);
}

extern "C" void destroy_mode(mode_interface* object)
{
  delete object;
}

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
		mSettings.add("Sound Reactive", "Audio", "1", 0.0, 1.0, ihmCheckbox);
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
	mBitmap->fill(mCurrentColor);
	mCurrentColor = inc_hue_of_color(mCurrentColor, 1);
}

void mode_fading::beat_detected()
{
	mCurrentColor = inc_hue_of_color(mCurrentColor, 80);
}
