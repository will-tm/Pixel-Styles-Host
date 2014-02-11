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
mode_touch::mode_touch(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable) : mode_interface(pWidth, pHeight, pName, pAudioAvailable)
{
	mIniFile = new ini_parser(mIniFilePath);
	mUI = uiSpectrum;
}

/*
 * destructor
 *
 */
mode_touch::~mode_touch()
{
	delete mIniFile;
}

/*
 * public functions
 *
 */
void mode_touch::paint()
{
	mColorsMutex.lock();

	rgb_color currentColor = mStaticColors[0];
	mBitmap->fill(currentColor);

	mColorsMutex.unlock();
}

void mode_touch::initialize(vector <rgb_color> pStaticColors)
{
	mColorsMutex.lock();
	mStaticColors = pStaticColors;
	mColorsMutex.unlock();
}

void mode_touch::touch(vector <rgb_color> pStaticColors, touch_type pTouchType)
{
	mColorsMutex.lock();
	mStaticColors = pStaticColors;
	mColorsMutex.unlock();
}
