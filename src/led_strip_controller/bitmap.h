/*
 * bitmap.h - Custom Bitmap class
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

#ifndef __BITMAP_H__
#define __BITMAP_H__

using namespace std;

#include <cstdio>
#include <iostream>
#include <vector>
#include <cmath>
#include "hsv_helper.h"

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
