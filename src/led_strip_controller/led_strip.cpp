/*
 * led_strip.cpp - LEDs Strip class dedicated to WS2801
 *
 * Copyright (C) 2013 William Markezana <william.markezana@me.com>
 *
 */

#include "led_strip.h"
#include <cstring>

/*
 * constructor
 *
 */
led_strip::led_strip(const char *pDevice, strip_bytes_order pBytesOrder, size_t pWidth, size_t pHeight)
{
	mWidth = pWidth;
	mHeight = pHeight;
	mBuffer.resize(mWidth * mHeight * 3);
	mSpi = new spi(pDevice);
	mTwoDimensions = true;
	mBytesOrder = pBytesOrder;
	mActive = true;
	
	set_gamma(0.25);
}

/*
 * constructor
 *
 */
led_strip::led_strip(const char *pDevice, strip_bytes_order pBytesOrder, size_t pLength)
{
	mWidth = pLength;
	mHeight = 1;
	mBuffer.resize(mWidth * 3);
	mSpi = new spi(pDevice);
	mTwoDimensions = false;
	mBytesOrder = pBytesOrder;
	
	set_gamma(0.25);
}

/*
 * destructor
 *
 */
led_strip::~led_strip()
{
	delete mSpi;
}

/*
 * private functions
 *
 */
void led_strip::fill_buffer_with_color(uint8_t *pBuffer, size_t &pIndex, rgb_color pColor)
{
	if (mBytesOrder == rgbBytesOrder)
	{
		pBuffer[pIndex++] = mGammaTable[pColor.R];
		pBuffer[pIndex++] = mGammaTable[pColor.G];
		pBuffer[pIndex++] = mGammaTable[pColor.B];
	}
	else
	{
		pBuffer[pIndex++] = mGammaTable[pColor.B];
		pBuffer[pIndex++] = mGammaTable[pColor.G];
		pBuffer[pIndex++] = mGammaTable[pColor.R];
	}
}

/*
 * public functions
 *
 */
void led_strip::set_gamma(float pGamma)
{
	float gamma;
	for (int i = 0; i < 256; i++)
	{
		gamma = pow(256.0f, pow((float) i / 255.0f, pGamma)) - 1.0f;
		if (gamma > 255.0f)
			gamma = 255.0f;
		if (gamma < 0.0f)
			gamma = 0.0f;
		mGammaTable[i] = (uint8_t) gamma;
	}
}

void led_strip::paint(bitmap *pBitmap, bool pReversed, bool pWaitForCompletion)
{
	size_t bufferPtr = 0;

	if (mActive)
	{
		if (!mTwoDimensions)
		{
			if (!pReversed)
			{
				for (size_t x = 0; x < mWidth; x++)
					fill_buffer_with_color(mBuffer.data(), bufferPtr, pBitmap->get_pixel(mWidth - 1 - x, 0));
			}
			else
			{
				for (size_t x = 0; x < mWidth; x++)
					fill_buffer_with_color(mBuffer.data(), bufferPtr, pBitmap->get_pixel(x, 0));
			}
		}
		else
		{
			if (!pReversed)
			{
				for (size_t y = 0; y < mHeight; y++)
				{
					if (y % 2 == 0)
					{
						for (size_t x = 0; x < mWidth; x++)
							fill_buffer_with_color(mBuffer.data(), bufferPtr, pBitmap->get_pixel(x, y));
					}
					else
					{
						for (size_t x = 0; x < mWidth; x++)
							fill_buffer_with_color(mBuffer.data(), bufferPtr, pBitmap->get_pixel(mWidth - 1 - x, y));
					}
				}
			}
			else
			{
				for (size_t y = mHeight - 1; y >= 0; y--)
				{
					if (y % 2 == 0)
					{
						for (size_t x = 0; x < mWidth; x++)
							fill_buffer_with_color(mBuffer.data(), bufferPtr, pBitmap->get_pixel(x, y));
					}
					else
					{
						for (size_t x = 0; x < mWidth; x++)
							fill_buffer_with_color(mBuffer.data(), bufferPtr, pBitmap->get_pixel(mWidth - 1 - x, y));
					}
				}
			}
		}
	}
	else
	{
		memset(&mBuffer.front(), 0, mBuffer.size());
	}
	
	mSpi->write_buffer(&mBuffer.front(), mBuffer.size());
	
	if (pWaitForCompletion)
		mSpi->waitForTransfertToComplete();
}
