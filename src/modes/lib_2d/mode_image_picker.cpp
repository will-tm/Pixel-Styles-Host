/*
 * mode_image_picker.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_image_picker.h"

/*
 * public library interface
 *
 */
extern "C" mode_interface* create_mode(size_t pWidth, size_t pHeight, bool pAudioAvailable)
{
  return new mode_image_picker(pWidth, pHeight, "Image picker", pAudioAvailable);
}

extern "C" void destroy_mode(mode_interface* object)
{
  delete object;
}
/* * constructor
 *
 */
mode_image_picker::mode_image_picker(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable)
: mode_interface(pWidth, pHeight, pName, pAudioAvailable)
{	
	mUI = uiImagePicker;
	mUdpBuffer = new uint8_t[mWidth * mHeight * 3];
}

/*
 * destructor
 *
 */
mode_image_picker::~mode_image_picker()
{
	
}

/*
 * public functions
 *
 */
void mode_image_picker::handle_udp_receive(uint8_t *pData, size_t pLength)
{
	if (pLength == mWidth * mHeight * 3)
	{
		memcpy(mUdpBuffer, pData, pLength);
	}
}

void mode_image_picker::paint()
{
	rgb_color color;
	int bufferPtr = 0;

	for (size_t y = 0; y < mHeight; y++)
	{
		for (size_t x = 0; x < mWidth; x++)
		{
			color.R = mUdpBuffer[bufferPtr++];
			color.G = mUdpBuffer[bufferPtr++];
			color.B = mUdpBuffer[bufferPtr++];
			mBitmap->set_pixel(x, y, color);
		}
	}
}
