/*
 * mode_balls.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_balls.h"

/*
 * public library interface
 *
 */
extern "C" mode_interface* create_mode(size_t pWidth, size_t pHeight, bool pAudioAvailable)
{
  return new mode_balls(pWidth, pHeight, "Balls", pAudioAvailable);
}

extern "C" void destroy_mode(mode_interface* object)
{
  delete object;
}

/*
 * constructor
 *
 */
mode_balls::mode_balls(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable)
		: mode_interface(pWidth, pHeight, pName, pAudioAvailable)
{
	mLastBallsCount = -1;

	// Settings;
	if (mAudioAvailable)
		mSettings.add("Sound Reactive", "Audio", "1", 0.0, 1.0, ihmCheckbox);
	mSettings.add("Count", "Balls", "5.0", 1.0f, 5.0f, ihmSpinEdit);
	mSettings.add("Enabled", "Blur", "1", 0.0f, 1.0f, ihmCheckbox);
	mSettings.add("Length", "Blur", "89.0", 0.0f, 100.0f, ihmLogTrackbar);
	mSettings.set_ini_path(mIniFilePath);
}

/*
 * destructor
 *
 */
mode_balls::~mode_balls()
{
	mBalls.clear();
}

/*
 * private functions
 *
 */
void mode_balls::set_balls_count(int count)
{
	mBalls.resize(count);
	for (size_t i = 0; i < mBalls.size(); i++)
	{
		mBalls[i].x = random(mWidth);
		mBalls[i].y = random(mHeight);
		mBalls[i].xWay = randomf(-0.2f, 0.2f);
		mBalls[i].yWay = randomf(-0.2f, 0.2f);
		mBalls[i].color = hue_to_rgb(random(360));
	}
}

/*
 * public functions
 *
 */
void mode_balls::paint()
{

	int ballsCount = mSettings["Count"]->get_value<int>();
	bool blurEffect = mSettings["Enabled"]->get_value<bool>();
	int blurLength = (int)mSettings["Length"]->max_value - mSettings["Length"]->get_value<int>();
	
	if (ballsCount != mLastBallsCount)
	{
		mLastBallsCount = ballsCount;
		set_balls_count(ballsCount);
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
	
	for (size_t i = 0; i < mBalls.size(); i++)
	{
		mBitmap->set_pixel((int) mBalls[i].x, (int) mBalls[i].y, mBalls[i].color);
		mBalls[i].x += mBalls[i].xWay;
		mBalls[i].y += mBalls[i].yWay;
		
		if (mBalls[i].x > mWidth - 1)
		{
			mBalls[i].x = (float) (mWidth - 1);
			mBalls[i].xWay *= -1.0f;
		}
		if (mBalls[i].y > mHeight - 1)
		{
			mBalls[i].y = (float) (mHeight - 1);
			mBalls[i].yWay *= -1.0f;
		}
		if (mBalls[i].x < 0)
		{
			mBalls[i].x = 1.0f;
			mBalls[i].xWay *= -1.0f;
		}
		if (mBalls[i].y < 0)
		{
			mBalls[i].y = 1.0f;
			mBalls[i].yWay *= -1.0f;
		}
	}
}

void mode_balls::beat_detected()
{
	set_balls_count(mLastBallsCount);
}
