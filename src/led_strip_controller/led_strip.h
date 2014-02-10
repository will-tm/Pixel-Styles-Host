/*
 * led_strip.h - LEDs Strip class dedicated to WS2801
 *
 * Copyright (C) 2013 William Markezana <william.markezana@me.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __LED_STRIP_H__
#define __LED_STRIP_H__

#include <cmath>
#include "spi.h"
#include "bitmap.h"

class led_strip
{
private:
	size_t mWidth;
	size_t mHeight;
	vector<unsigned char> mBuffer;
	uint8_t mGammaTable[256];
	spi *mSpi;
	bool mTwoDimensions;
public:
	led_strip(const char *pDevice, size_t pLength);
	led_strip(const char *pDevice, size_t pWidth, size_t pHeight);
	~led_strip();

	void set_gamma(float pGamma);
	void paint(bitmap *pBitmap, bool pReversed, bool pWaitForCompletion);
	size_t size() { return mWidth; }
};

#endif
