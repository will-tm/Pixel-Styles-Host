/*
 * mode_space_invaders.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_space_invaders.h"

#define ANIMATE_DELAY					(500000) // us
#define INVADERS_WIDTH 					(11)
#define INVADERS_HEIGHT					(8)
#define INVADERS_COUNT					(2)

const uint8_t mode_space_invaders::pixels[] =
{
	0,0,1,0,0,0,0,0,1,0,0,
	0,0,0,1,0,0,0,1,0,0,0,
	0,0,1,1,1,1,1,1,1,0,0,
	0,1,1,0,1,1,1,0,1,1,0,
	1,1,1,1,1,1,1,1,1,1,1,
	1,0,1,1,1,1,1,1,1,0,1,
	1,0,1,0,0,0,0,0,1,0,1,
	0,0,0,1,1,0,1,1,0,0,0,

	0,0,1,0,0,0,0,0,1,0,0,
	1,0,0,1,0,0,0,1,0,0,1,
	1,0,1,1,1,1,1,1,1,0,1,
	1,1,1,0,1,1,1,0,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,
	0,1,1,1,1,1,1,1,1,1,0,
	0,0,1,0,0,0,0,0,1,0,0,
	0,1,0,0,0,0,0,0,0,1,0
};

/*
 * public library interface
 *
 */
extern "C" mode_interface* create_mode(size_t pWidth, size_t pHeight, bool pAudioAvailable, vector<size_t> pSegments)
{
  return new mode_space_invaders(pWidth, pHeight, "Space Invaders", pAudioAvailable, pSegments);
}

extern "C" void destroy_mode(mode_interface* object)
{
  delete object;
}

/*
 * constructor
 *
 */
mode_space_invaders::mode_space_invaders(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable, vector<size_t> pSegments)
		: mode_interface(pWidth, pHeight, pName, pAudioAvailable, pSegments)
{
	mLastTick = 0;
	mImageState = 0;
	
	// Settings
	mSettings.add("Color", "Invader", rgb_to_string(ColorGreen), 0.0f, 4294967295.0f, ihmColorSelector);
	mSettings.set_ini_path(mIniFilePath);
}

/*
 * destructor
 *
 */
mode_space_invaders::~mode_space_invaders()
{
	
}

/*
 * public functions
 *
 */
void mode_space_invaders::paint()
{
	rgb_color color = int_to_rgb(mSettings["Color"]->get_value<uint32_t>());
	int xOffset = (mWidth - INVADERS_WIDTH) / 2;
	int yOffset = (mHeight - INVADERS_HEIGHT) / 2;

	size_t imageOffset = INVADERS_WIDTH * INVADERS_HEIGHT * mImageState;
	for (size_t y = 0; y < INVADERS_HEIGHT; y++)
	{
		size_t rowOffset = y * INVADERS_WIDTH;
		for (size_t x = 0; x < INVADERS_WIDTH; x++)
		{
			mBitmap->set_pixel(xOffset + x, yOffset + y, pixels[x + rowOffset + imageOffset] == 1 ? color : ColorBlack);
		}
	}

	if (get_tick_us() - mLastTick > ANIMATE_DELAY)
	{
		mLastTick = get_tick_us();
		if (++mImageState >= INVADERS_COUNT)
			mImageState = 0;
	}
}
