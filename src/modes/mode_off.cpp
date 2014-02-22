/*
 * mode_off.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_off.h"

/*
 * public library interface
 *
 */
extern "C" mode_interface* create_mode(size_t pWidth, size_t pHeight, bool pAudioAvailable)
{
  return new mode_off(pWidth, pHeight, "OFF", pAudioAvailable);
}

extern "C" void destroy_mode(mode_interface* object)
{
  delete object;
}

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
