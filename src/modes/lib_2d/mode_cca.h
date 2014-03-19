/*
 * mode_cca.h
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
class mode_cca: public mode_interface
{
private:
	bitmap *mGridLayer;
	vector<vector<uint16_t> > mData;
	vector<vector<uint16_t> > mDataBuffer;
	uint8_t mIterations;
	uint8_t mEmptyFrames;

	void setup_grid_layer_with_alpha(uint8_t alpha);
public:
	mode_cca(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable, vector<size_t> pSegments);
	~mode_cca();

	virtual void paint();
	virtual void beat_detected();
};
