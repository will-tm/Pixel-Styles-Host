/*
 * mode_off.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_off.h"

/*
 * constructor
 *
 */
mode_off::mode_off(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable)
		: mode_interface(pWidth, pHeight, pName, pAudioAvailable)
{
	
}

/*
 * destructor
 *
 */
mode_off::~mode_off()
{
	
}

/*
 * public functions
 *
 */
void mode_off::paint()
{
	mBitmap->fill(ColorBlack);
}
