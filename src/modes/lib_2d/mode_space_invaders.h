/*
 * mode_space_invaders.h
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
class mode_space_invaders: public mode_interface
{
private:
	uint32_t mLastTick;
	uint8_t mImageState;
public:
	static uint8_t const pixels[];

	mode_space_invaders(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable, vector<size_t> pSegments);
	~mode_space_invaders();

	virtual void paint();
};
