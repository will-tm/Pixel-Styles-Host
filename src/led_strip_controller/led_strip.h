/*
 * led_strip.h - LEDs Strip class dedicated to WS2801
 *
 * Copyright (C) 2013 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

#include <cmath>
#include "spi.h"
#include "bitmap.h"

/*
 * public types
 *
 */
typedef enum
{
	rgbBytesOrder,
	bgrBytesOrder
} strip_bytes_order;

/*
 * public class
 *
 */
class led_strip
{
private:
	size_t mWidth;
	size_t mHeight;
	vector<uint8_t> mBuffer;
	uint8_t mGammaTable[256];
	spi *mSpi;
	bool mTwoDimensions;
	strip_bytes_order mBytesOrder;

	void fill_buffer_with_color(uint8_t *pBuffer, size_t &pIndex, rgb_color pColor);
public:
	led_strip(const char *pDevice, strip_bytes_order pBytesOrder, size_t pLength);
	led_strip(const char *pDevice, strip_bytes_order pBytesOrder, size_t pWidth, size_t pHeight);
	~led_strip();

	void set_gamma(float pGamma);
	void paint(bitmap *pBitmap, bool pReversed, bool pWaitForCompletion);
	size_t size()
	{
		return mWidth;
	}
};
