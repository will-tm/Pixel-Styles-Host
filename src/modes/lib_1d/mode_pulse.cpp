/*
 * mode_pulse.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_pulse.h"

/*
 * public library interface
 *
 */
extern "C" mode_interface* create_mode(size_t pWidth, size_t pHeight, bool pAudioAvailable, vector<size_t> pSegments)
{
	return new mode_pulse(pWidth, pHeight, "Pulse", pAudioAvailable, pSegments);
}

extern "C" void destroy_mode(mode_interface* object)
{
	delete object;
}

/*
 * constructor
 *
 */
mode_pulse::mode_pulse(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable, vector<size_t> pSegments) : mode_interface(pWidth, pHeight, pName, pAudioAvailable, pSegments)
{
	mSectors.resize(mSegments.size());
	for(size_t i = 0; i < mSectors.size(); i++)
	{
		configure_sector(&mSectors[i],  mSegments[i]);
	}
	configure_sector(&mLinkedSector,  mSize);

	// Settings
	if(mAudioAvailable) mSettings.add("Sound Reactive", 		"Audio", 	"True", 0.0,  1.0,   ihmCheckbox);
						mSettings.add("Pulse Speed",			"Colors",  	"10",   1.0,  100.0, ihmTrackbar);
						mSettings.add("Link All", 				"Colors", 	"True", 0.0,  1.0, 	 ihmCheckbox);

	mSettings.set_ini_path(mIniFilePath);
}

/*
 * destructor
 *
 */
mode_pulse::~mode_pulse()
{

}

/*
 * private functions
 *
 */
void mode_pulse::configure_sector(mode_pulse_sector *pSector, size_t pSize)
{
	pSector->huePtr = randomf(0.0f, 359.0f);
	pSector->color = hue_float_to_rgb(pSector->huePtr);
	pSector->satPtr = 10.0f;
	pSector->size = pSize;
	pSector->state = pulseUp;
	pSector->increment = 0.3f;
}

void mode_pulse::sector_tasks(mode_pulse_sector *pSector)
{
	mSpeed = mSettings["Pulse Speed"]->get_value<float>();
	pSector->increment = mSpeed / 100.0f;

	if(pSector->satPtr >= 255.0f  && pSector->state == pulseUp)
	{
		pSector->satPtr = 255.0f;
		pSector->state = pulseDown;
	}
	if(pSector->satPtr <= 20.0f  && pSector->state == pulseDown)
	{
		pSector->satPtr = 20.0f;
		pSector->state = pulseUp;

		pSector->huePtr = pSector->huePtr + randomf(40.0f,319.9f);
		if(pSector->huePtr > 359.0f) pSector->huePtr = pSector->huePtr - 359.0f;
	}

	if(pSector->state == pulseUp) pSector->satPtr = pSector->satPtr + pSector->increment;
	if(pSector->state == pulseDown) pSector->satPtr = pSector->satPtr - pSector->increment;

	float sat = pSector->satPtr * pSector->satPtr / 255.0f;
	pSector->color = hue_val_float_to_rgb(pSector->huePtr, sat);
}

void mode_pulse::sectors_tasks()
{
	for(size_t i = 0; i < mSectors.size(); i++)
	{
		sector_tasks(&mSectors[i]);
	}
}

/*
 * public functions
 *
 */
void mode_pulse::paint()
{
	size_t sectorOffset = 0;
	size_t sectorIndex = 0;

	mLinkedSectors = mSettings["Link All"]->get_value<bool>();
	if(mLinkedSectors)
	{
		mBitmap->fill(mLinkedSector.color);
		sector_tasks(&mLinkedSector);
	}
	else
	{
		for(size_t i = 0; i < mSize; i++)
		{
			if((i-sectorOffset) >= mSectors[sectorIndex].size)
			{
				sectorOffset += mSectors[sectorIndex].size;
				sectorIndex++;
			}
			mBitmap->set_pixel(i, 0, mSectors[sectorIndex].color);
		}
		sectors_tasks();
	}
}

void mode_pulse::beat_detected()
{
	if(!mAudioAvailable) return;

	mSoundReactive = mSettings["Sound Reactive"]->get_value<bool>();
	if(mSoundReactive)
	{

	}
}

void mode_pulse::initialize(vector <rgb_color> pStaticColors)
{

}
