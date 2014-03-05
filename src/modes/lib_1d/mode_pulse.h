/*
 * mode_pulse.h
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
typedef enum
{
	pulseUp,
	pulseDown
}
mode_pulse_state;

typedef struct
{
	mode_pulse_state state;
	float huePtr;
	float satPtr;
	float increment;
	rgb_color color;
	size_t size;
}
mode_pulse_sector;

/*
 * public class
 *
 */
class mode_pulse : public mode_interface
{
private:
	vector<mode_pulse_sector> mSectors;
	mode_pulse_sector mLinkedSector;
	bool mLinkedSectors;
	void configure_sector(mode_pulse_sector *pSector, size_t pSize);
	void sector_tasks(mode_pulse_sector *pSector);
	void sectors_tasks();
public:
	mode_pulse(size_t pSize, string pName, bool pAudioAvailable, vector<size_t> pSegments);
	~mode_pulse();

	virtual void paint();
	virtual void beat_detected();
	virtual void initialize(vector <rgb_color> pStaticColors);
};
