/*
 * mode_lines.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_lines.h"

/*
 * public library interface
 *
 */
extern "C" mode_interface* create_mode(size_t pWidth, size_t pHeight, bool pAudioAvailable, vector<size_t> pSegments)
{
  return new mode_lines(pWidth, pHeight, "Lines", pAudioAvailable, pSegments);
}

extern "C" void destroy_mode(mode_interface* object)
{
  delete object;
}

/*
 * constructor
 *
 */
mode_lines::mode_lines(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable, vector<size_t> pSegments)
		: mode_interface(pWidth, pHeight, pName, pAudioAvailable, pSegments)
{
	mLastCornersCount = -1;
	mCurrentColor = hue_to_rgb(random(360));
	
	// Settings;
	if (mAudioAvailable)
		mSettings.add("Sound Reactive", "Audio", "1", 0.0, 1.0, ihmCheckbox);
	mSettings.add("Count", "Corners", "5.0", 1.0f, 5.0f, ihmSpinEdit);
	mSettings.add("Enabled", "Blur", "1", 0.0f, 1.0f, ihmCheckbox);
	mSettings.add("Length", "Blur", "89.0", 0.0f, 100.0f, ihmLogTrackbar);
	mSettings.add("Fading", "Color", "1", 0.0f, 1.0f, ihmCheckbox);
	mSettings.set_ini_path(mIniFilePath);
}

/*
 * destructor
 *
 */
mode_lines::~mode_lines()
{
	mCorners.clear();
}

/*
 * private functions
 *
 */
void mode_lines::set_corners_count(int count)
{
	if (count < 1)
		count = 1;

	mCorners.resize(count);
	for (size_t i = 0; i < mCorners.size(); i++)
	{
		mCorners[i].x = random(mWidth);
		mCorners[i].y = random(mHeight);
		mCorners[i].xWay = randomf(-0.2f, 0.2f);
		mCorners[i].yWay = randomf(-0.2f, 0.2f);
	}
}

/*
 * public functions
 *
 */
void mode_lines::paint()
{
	int cornersCount = mSettings["Count"]->get_value<int>();
	bool blurEffect = mSettings["Enabled"]->get_value<bool>();
	int blurLength = (int)mSettings["Length"]->max_value - mSettings["Length"]->get_value<int>();
	bool fadingColors = mSettings["Fading"]->get_value<bool>();
	
	if (cornersCount != mLastCornersCount)
	{
		mLastCornersCount = cornersCount;
		set_corners_count(cornersCount);
		mCurrentColor = hue_to_rgb(random(360));
	}
	
	if (!blurEffect)
	{
		mBitmap->fill(ColorBlack);
	}
	else
	{
		for (size_t y = 0; y < mHeight; y++)
		{
			for (size_t x = 0; x < mWidth; x++)
			{
				rgb_color color = mBitmap->get_pixel(x, y);
				if (color.R >= blurLength)
					color.R -= blurLength;
				else color.R = 0;
				if (color.G >= blurLength)
					color.G -= blurLength;
				else color.G = 0;
				if (color.B >= blurLength)
					color.B -= blurLength;
				else color.B = 0;
				mBitmap->set_pixel(x, y, color);
			}
		}
	}
	
	for (size_t i = 0; i < mCorners.size(); i++)
	{
		if (i == 0)
			mBitmap->move_to((int) mCorners[i].x, (int) mCorners[i].y);
		else
			mBitmap->line_to((int) mCorners[i].x, (int) mCorners[i].y, mCurrentColor);

		mCorners[i].x += mCorners[i].xWay;
		mCorners[i].y += mCorners[i].yWay;
		
		if (mCorners[i].x > mWidth - 1)
		{
			mCorners[i].x = (float) (mWidth - 1);
			mCorners[i].xWay *= -1.0f;
		}
		if (mCorners[i].y > mHeight - 1)
		{
			mCorners[i].y = (float) (mHeight - 1);
			mCorners[i].yWay *= -1.0f;
		}
		if (mCorners[i].x < 0)
		{
			mCorners[i].x = 1.0f;
			mCorners[i].xWay *= -1.0f;
		}
		if (mCorners[i].y < 0)
		{
			mCorners[i].y = 1.0f;
			mCorners[i].yWay *= -1.0f;
		}
	}
	mBitmap->line_to((int) mCorners[0].x, (int) mCorners[0].y, mCurrentColor);

	if (fadingColors)
	{
		mCurrentColor = inc_hue_of_color(mCurrentColor, 1);
	}
}

void mode_lines::beat_detected()
{
	set_corners_count(mLastCornersCount);
	mCurrentColor = inc_hue_of_color(mCurrentColor, 120);
}
