/*
 * mode_rainbow.h
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
class mode_rainbow : public mode_interface
{
private:
	float mRotationPtr;
	hsv_color mRotationStartColor;
public:
	mode_rainbow(size_t pSize, string pName, bool pAudioAvailable, vector<size_t> pSegments);
	~mode_rainbow();

	virtual void paint();
	virtual void beat_detected();
	virtual void initialize(vector <rgb_color> pStaticColors);
};
