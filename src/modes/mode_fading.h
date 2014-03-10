/*
 * mode_fading.h
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
class mode_fading: public mode_interface
{
private:
	float mRotationPtr;
	hsv_color mRotationStartColor;
	bool mLinkedSectors;
public:
	mode_fading(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable, vector<size_t> pSegments);
	~mode_fading();

	virtual void paint();
	virtual void beat_detected();
	virtual void initialize(vector <rgb_color> pStaticColors);
};
