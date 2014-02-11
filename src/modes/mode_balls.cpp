/*
 * mode_balls.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_balls.h"

/*
 * constructor
 *
 */
mode_balls::mode_balls(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable) : mode_interface(pWidth, pHeight, pName, pAudioAvailable)
{
	mLastBallsCount = -1;

	mSettings.add("Reset", 		"Balls", 	"False", 	0.0f,  1.0f, 	 ihmButton);
	mSettings.add("Count", 		"Balls", 	"5.0"	, 	1.0f,  5.0f, 	 ihmSpinEdit);
	mSettings.add("Enabled", 	"Blur", 	"True", 	0.0f,  1.0f, 	 ihmCheckbox);
	mSettings.add("Length", 	"Blur", 	"89.0",		0.0f,  100.0f, ihmLogTrackbar);
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
	for(size_t i = 0; i < mBalls.size(); i++)
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
	bool resetBalls = mSettings["Reset"]->get_value<bool>();
	int ballsCount = mSettings["Count"]->get_value<int>();
	bool blurEffect = mSettings["Enabled"]->get_value<bool>();
	int blurLength = mSettings["Length"]->get_value<bool>();

	if(resetBalls || (ballsCount != mLastBallsCount))
	{
		mLastBallsCount = ballsCount;
		mSettings["Reset"]->set_value<bool>(false);

		set_balls_count(ballsCount);
	}

	if(!blurEffect)
	{
		mBitmap->fill(ColorBlack);
	}
	else
	{
		for(int y = 0; y < mHeight; y++)
		{
			for(int x = 0; x < mWidth; x++)
			{
				rgb_color color = mBitmap->get_pixel(x,y);
				if(color.R >= blurLength) color.R -= blurLength; else color.R = 0;
				if(color.G >= blurLength) color.G -= blurLength; else color.G = 0;
				if(color.B >= blurLength) color.B -= blurLength; else color.B = 0;
				mBitmap->set_pixel(x,y,color);
			}
		}
	}

	for(uint i = 0; i < mBalls.size(); i++)
	{
		mBitmap->set_pixel((int)mBalls[i].x, (int)mBalls[i].y, mBalls[i].color);
		mBalls[i].x += mBalls[i].xWay;
		mBalls[i].y += mBalls[i].yWay;

		if(mBalls[i].x > mWidth-1)
		{
			mBalls[i].x = (float)(mWidth-1);
			mBalls[i].xWay *= -1.0f;
		}
		if(mBalls[i].y > mHeight-1)
		{
			mBalls[i].y = (float)(mHeight-1);
			mBalls[i].yWay *= -1.0f;
		}
		if(mBalls[i].x < 0)
		{
			mBalls[i].x = 1.0f;
			mBalls[i].xWay *= -1.0f;
		}
		if(mBalls[i].y < 0)
		{
			mBalls[i].y = 1.0f;
			mBalls[i].yWay *= -1.0f;
		}
	}
}
