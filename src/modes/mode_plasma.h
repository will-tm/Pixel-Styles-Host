/*
 * mode_plasma.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

using namespace std;

#include "mode_interface.h"

/*
 * public class
 *
 */
class mode_plasma : public mode_interface
{
private:
	vector<vector<uint16_t> > mHueMatrix;
public:
	mode_plasma(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable);
	~mode_plasma();

	virtual void paint();
	virtual void beat_detected();
};
