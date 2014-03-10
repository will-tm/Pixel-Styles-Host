/*
 * mode_balls.h
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
	rgb_color color;
} ball_t;

/*
 * public class
 *
 */
class mode_balls: public mode_interface
{
private:
	vector<ball_t> mBalls;
	int mLastBallsCount;
	void set_balls_count(int count);
public:
	mode_balls(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable, vector<size_t> pSegments);
	~mode_balls();

	virtual void paint();
	virtual void beat_detected();
};
