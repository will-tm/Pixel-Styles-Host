/*
 * led_strip.c - LEDs Strip class dedicated to WS2801
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

#include "led_strip.h"

led_strip::led_strip(const char *pDevice, size_t pWidth, size_t pHeight)
{
	mWidth = pWidth;
	mHeight = pHeight;
	mBuffer.resize(mWidth * mHeight * 3);
	mSpi = new spi(pDevice);
	mTwoDimensions = true;

	set_gamma(0.25);
}

led_strip::led_strip(const char *pDevice, size_t pLength)
{
	mWidth = pLength;
	mHeight = 1;
	mBuffer.resize(mWidth * 3);
	mSpi = new spi(pDevice);
	mTwoDimensions = false;

	set_gamma(0.25);
}

led_strip::~led_strip()
{
	delete mSpi;
}

void led_strip::set_gamma(float pGamma)
{
	float gamma;
	for (int i = 0; i < 256; i++)
	{
		gamma = pow(256.0f,pow((float)i/255.0f,pGamma))-1.0f;
		if (gamma > 255.0f) gamma = 255.0f;
		if (gamma <  0.0f) gamma = 0.0f;
		mGammaTable[i] = (uint8_t)gamma;
	}
}

void led_strip::paint(bitmap *pBitmap, bool pReversed, bool pWaitForCompletion)
{
	int bufferPtr = 0;
	rgb_color CurrentColor;

	if (!mTwoDimensions)
	{
		if (!pReversed)
		{
			for (size_t x = 0; x < mWidth; x++)
			{
				CurrentColor = pBitmap->get_pixel(x,0);

				mBuffer[bufferPtr++] = mGammaTable[CurrentColor.B];
				mBuffer[bufferPtr++] = mGammaTable[CurrentColor.G];
				mBuffer[bufferPtr++] = mGammaTable[CurrentColor.R];
			}
		}
		else
		{
			for (size_t x = 0; x < mWidth; x++)
			{
				CurrentColor = pBitmap->get_pixel(mWidth-1-x,0);

				mBuffer[bufferPtr++] = mGammaTable[CurrentColor.B];
				mBuffer[bufferPtr++] = mGammaTable[CurrentColor.G];
				mBuffer[bufferPtr++] = mGammaTable[CurrentColor.R];
			}
		}
	}
	else
	{
		if (!pReversed)
		{
			for (size_t y = 0; y < mHeight; y++)
			{
				if (y%2 == 0)
				{
					for (size_t x = 0; x < mWidth; x++)
					{
						CurrentColor = pBitmap->get_pixel(x,y);

						mBuffer[bufferPtr++] = mGammaTable[CurrentColor.B];
						mBuffer[bufferPtr++] = mGammaTable[CurrentColor.G];
						mBuffer[bufferPtr++] = mGammaTable[CurrentColor.R];
					}
				}
				else
				{
					for (size_t x = 0; x < mWidth; x++)
					{
						CurrentColor = pBitmap->get_pixel(mWidth-1-x,y);

						mBuffer[bufferPtr++] = mGammaTable[CurrentColor.B];
						mBuffer[bufferPtr++] = mGammaTable[CurrentColor.G];
						mBuffer[bufferPtr++] = mGammaTable[CurrentColor.R];
					}
				}
			}
		}
		else
		{
			for (size_t y = mHeight - 1; y >= 0; y--)
			{
				if (y%2 == 0)
				{
					for (size_t x = 0; x < mWidth; x++)
					{
						CurrentColor = pBitmap->get_pixel(x,y);

						mBuffer[bufferPtr++] = mGammaTable[CurrentColor.B];
						mBuffer[bufferPtr++] = mGammaTable[CurrentColor.G];
						mBuffer[bufferPtr++] = mGammaTable[CurrentColor.R];
					}
				}
				else
				{
					for (size_t x = 0; x < mWidth; x++)
					{
						CurrentColor = pBitmap->get_pixel(mWidth-1-x,y);

						mBuffer[bufferPtr++] = mGammaTable[CurrentColor.B];
						mBuffer[bufferPtr++] = mGammaTable[CurrentColor.G];
						mBuffer[bufferPtr++] = mGammaTable[CurrentColor.R];
					}
				}
			}
		}
	}

	mSpi->write_buffer(&mBuffer.front(), mBuffer.size());

	if (pWaitForCompletion)
		mSpi->waitForTransfertToComplete();
}
