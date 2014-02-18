/*
 * mode_lines.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

using namespace std;

#include "mode_interface.h"

/*
 * public types
 *
 */
typedef struct
{
	float x;
	float y;
	float xWay;
	float yWay;
} corner_t;
/*
 * public class
 *
 */
class mode_lines: public mode_interface
{
private:
	vector<corner_t> mCorners;
	int mLastCornersCount;
	rgb_color mCurrentColor;
	void set_corners_count(int count);
public:
	mode_lines(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable);
	~mode_lines();

	virtual void paint();
	virtual void beat_detected();
};
