/*
 * mode_plasma2.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

using namespace std;

#include "mode_interface.h"

#include <cmath>

/*
 * public class
 *
 */
class mode_plasma2: public mode_interface
{
private:
	vector<vector<uint16_t> > mHueMatrix;
	int mFrameCount;
public:
	mode_plasma2(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable, vector<size_t> pSegments);
	~mode_plasma2();

	virtual void paint();
	virtual void beat_detected();
};
