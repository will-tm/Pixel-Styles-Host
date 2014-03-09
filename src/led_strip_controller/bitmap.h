/*
 * bitmap.h - Custom Bitmap class
 *
 * Copyright (C) 2013 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

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
	vector<bitmap*> mLayers;

	int mLocationX;
	int mLocationY;

	void draw_circle_helper( int x0, int y0, int r, uint8_t cornername, rgb_color pColor);
	void fill_circle_helper(int x0, int y0, int r, uint8_t cornername, int delta, rgb_color pColor);
	void draw_fast_vertical_line(int x, int y, int h, rgb_color pColor);
	void draw_fast_horizontal_line(int x, int y, int w, rgb_color pColor);
	void draw_char(int x, int y, unsigned char c, rgb_color pColor);
public:
	int width;
	int height;

	bitmap(int pWidth, int pHeight);
	~bitmap();

	rgb_color get_pixel(int x, int y);
	void set_pixel(int x, int y, rgb_color pColor);

	hsv_color get_hsv_pixel(int x, int y);
	void set_hsv_pixel(int x, int y, hsv_color pColor);

	void fill(rgb_color pColor);
	void clear();

	void move_to(int x, int y);
	void line_to(int x, int y, rgb_color pColor);
	void draw_circle(int x0, int y0, int r, rgb_color pColor);
	void fill_circle(int x0, int y0, int r, rgb_color pColor);
	void draw_rect(int x, int y, int w, int h, rgb_color pColor);
	void fill_rect(int x, int y, int w, int h, rgb_color pColor);
	void draw_round_rect(int x, int y, int w, int h, int r, rgb_color pColor);
	void fill_round_rect(int x, int y, int w, int h, int r, rgb_color pColor);
	void print_string(int x, int y, string s, rgb_color pColor);

	void assign(bitmap *pBitmap);
	string to_string();

	bitmap *add_layer();
	void render();
};

