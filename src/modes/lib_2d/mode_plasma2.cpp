/*
 * mode_plasma2.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_plasma2.h"

/*
 * public library interface
 *
 */
extern "C" mode_interface* create_mode(size_t pWidth, size_t pHeight, bool pAudioAvailable, vector<size_t> pSegments)
{
	return new mode_plasma2(pWidth, pHeight, "Plasma2", pAudioAvailable, pSegments);
}

extern "C" void destroy_mode(mode_interface* object)
{
	delete object;
}

/*
 * constructor
 *
 */
mode_plasma2::mode_plasma2(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable, vector<size_t> pSegments)
		: mode_interface(pWidth, pHeight, pName, pAudioAvailable, pSegments)
{
	mFrameCount = 1;

	mHueMatrix.resize(mWidth);
	for (auto &column : mHueMatrix)
	{
		column.resize(mHeight);
		for (auto &item : column)
			item = 0;
	}
}

/*
 * destructor
 *
 */
mode_plasma2::~mode_plasma2()
{
	
}

/*
 * public functions
 *
 */
void mode_plasma2::paint()
{
	int aaa = 128;
	hsv_color hsvColor;
	int timeDisplacement = mFrameCount++;

	float calculation1 = sinf( M_PI / 180.0 * (timeDisplacement * 0.61655617f));
	float calculation2 = sinf( M_PI / 180.0 * (timeDisplacement * -3.6352262f));

	float  xc = 5;
	for (int x = 0; x < mWidth; x++, xc++)
	{
		float yc = 5;
		float s1 = aaa + aaa * sinf(M_PI / 180.0 * (xc) * calculation1 );

		for (int y = 0; y < mHeight; y++, yc++)
		{
			float s2 = aaa + aaa * sinf(M_PI / 180.0f * (yc) * calculation2 );
			float s3 = aaa + aaa * sinf(M_PI / 180.0f * ((xc + yc + timeDisplacement * 3.0f) / 2.0f));  
			float s  = (s1+ s2 + s3) / 360.0f;

			int aa = (int)(s*360+0.5f);

			hsvColor.H = aa % 360;
			hsvColor.S = 255;
			hsvColor.V = 255;
			
			mBitmap->set_hsv_pixel(x, y, hsvColor);
		}
	} 
}

void mode_plasma2::beat_detected()
{

}
