/*
 * bitmap.h - Custom Bitmap class
 *
 * Copyright (C) 2013 William Markezana <william.markezana@me.com>
 *
 */

#ifndef __BITMAP_H__
#define __BITMAP_H__

using namespace std;

#include <cstdio>
#include <iostream>
#include <vector>
#include <cmath>
#include "hsv_helper.h"

/*
 * public class
 *
 */
class bitmap
{
private:
  	vector<vector<rgb_color> > mMemory;

	int mLocationX;
	int mLocationY;
public:
	int	width;
	int height;

	bitmap(int pWidth, int pHeight);
	~bitmap();

	rgb_color get_pixel(int x, int y);
	void set_pixel(int x, int y, rgb_color Color);

	hsv_color get_hsv_pixel(int x, int y);
	void set_hsv_pixel(int x, int y, hsv_color Color);

	void fill(rgb_color aColor);
	void clear();

	void move_to(int x, int y);
	void line_to(int x, int y, rgb_color Color);

	void assign(bitmap *__Bitmap);
	string to_string();
};

/********************************************************************************************/
/* END OF FILE																				*/
/********************************************************************************************/
#endif
