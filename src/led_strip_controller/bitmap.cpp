/*
 * bitmap.cpp - Custom Bitmap class
 *
 * Copyright (C) 2013 William Markezana <william.markezana@me.com>
 *
 */

#include "bitmap.h"

/*
 * constructor
 *
 */
bitmap::bitmap(int pWidth, int pHeight)
{
	width = pWidth;
	height = pHeight;

	mMemory.resize(width);
	for(int i = 0; i < width; i++)
		mMemory[i].resize(height);
	clear();
}

/*
 * destructor
 *
 */
bitmap::~bitmap()
{
	mMemory.clear();
}

/*
 * public functions
 *
 */
void bitmap::fill(rgb_color aColor)
{
  	for(int y = 0; y < height; y++)
    {
		for(int x = 0; x < width; x++)
		{
			mMemory[x][y] = aColor;
		}
	}
}

void bitmap::clear()
{
  	rgb_color NullColor = {0x00,0x00,0x00,0x00};
	fill(NullColor);
}

rgb_color bitmap::get_pixel(int x, int y)
{
  	return(mMemory[x][y]);
}

void bitmap::set_pixel(int x, int y, rgb_color Color)
{
	if(x >= width) return;
	if(y >= height) return;
	mMemory[x][y] = Color;
}

hsv_color bitmap::get_hsv_pixel(int x, int y)
{
	return(rgb_to_hsv(mMemory[x][y]));
}

void bitmap::set_hsv_pixel(int x, int y, hsv_color Color)
{
	if(x >= width) return;
	if(y >= height) return;
	mMemory[x][y] = hsv_to_rgb(Color);
}

void bitmap::move_to(int x, int y)
{
	mLocationX = x;
	mLocationY = y;
}

void bitmap::line_to(int x, int y, rgb_color Color)
{
    float x1, x2, y1, y2;

	x1 = (float)mLocationX;
	y1 = (float)mLocationY;
	x2 = (float)x;
	y2 = (float)y;

	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if(steep)
	{
		swap(x1, y1);
		swap(x2, y2);
	}

	if(x1 > x2)
	{
		swap(x1, x2);
		swap(y1, y2);
	}

	const float dx = x2 - x1;
	const float dy = fabs(y2 - y1);

	float error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	int y0 = (int)(y1+0.5f);

	const int maxX = (int)(x2+0.5f);

	for(int x0=(int)x1; x0<=maxX; x0++)
	{
		if(steep)
        {
        	set_pixel(y0,x0, Color);
		}
		else
		{
			set_pixel(x0,y0, Color);
		}

		error -= dy;
		if(error < 0)
		{
			y0 += ystep;
			error += dx;
		}
	}

	mLocationX = x;
	mLocationY = y;
}

void bitmap::assign(bitmap *__Bitmap)
{
	for(int y = 0; y < height; y++)
    {
		for(int x = 0; x < width; x++)
		{
			set_pixel(x,y, __Bitmap->get_pixel(x,y));
		}
	}
}

string bitmap::to_string()
{
	char buf[16];
	string result;
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			sprintf(buf,"%2.2X%2.2X%2.2X", mMemory[x][y].B, mMemory[x][y].G, mMemory[x][y].R);
			result.append(buf);
		}
	}
	return result;
}
