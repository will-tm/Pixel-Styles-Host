/*
 * mode_bounce.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_bounce.h"

/*
 * public library interface
 *
 */
extern "C" mode_interface* create_mode(size_t pWidth, size_t pHeight, bool pAudioAvailable, vector<size_t> pSegments)
{
  return new mode_bounce(pWidth, pHeight, "Bounce", pAudioAvailable, pSegments);
}

extern "C" void destroy_mode(mode_interface* object)
{
  delete object;
}

/*
 * constructor
 *
 */
mode_bounce::mode_bounce(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable, vector<size_t> pSegments) : mode_interface(pWidth, pHeight, pName, pAudioAvailable, pSegments)
{
	mPointers.resize(mSegments.size());
	for(size_t i = 0; i < mPointers.size(); i++)
	{
		configure_pointer(&mPointers[i], mSegments[i]);
	}
	configure_pointer(&mLinkedPointer, mSize);

	pointers_tasks(0.0f);

	// Settings
	if(mAudioAvailable) mSettings.add("Sound Reactive", 		"Audio", 	"True", 0.0,  1.0, 	 ihmCheckbox);
						mSettings.add("Link All", 				"Colors", 	"False",0.0,  1.0, 	 ihmCheckbox);
	mSettings.set_ini_path(mIniFilePath);
}

/*
 * destructor
 *
 */
mode_bounce::~mode_bounce()
{

}

/*
 * private functions
 *
 */
void mode_bounce::configure_pointer(mode_bounce_pointer *pPointer, size_t pSize)
{
	pPointer->size = pSize;
	pPointer->position = -1.0f;
	pPointer->increasing = -1.0f;
	pPointer->rotationPtr = randomf(0.0f, 359.0f);
	pPointer->speed = randomf(0.3f, 3.0f);
	pPointer->colorSpeed = randomf(20.0f, 50.0f);
	pPointer->acceleration = 0.0f;
	pPointer->colorAcceleration = 0.0f;
	pPointer->color = hue_float_to_rgb(pPointer->rotationPtr);
	pPointer->nextColor = hue_float_to_rgb(pPointer->rotationPtr);
}

void mode_bounce::pointer_tasks(mode_bounce_pointer *pPointer, float pStep)
{
	pPointer->position = pPointer->position + pStep * pPointer->increasing * (pPointer->speed + pPointer->acceleration);

	if(pPointer->position < 0.0f) pPointer->position = 0.0f;
	if(pPointer->position > 1.0f) pPointer->position = 1.0f;

	if(pPointer->position == 0.0f || pPointer->position == 1.0f)
	{
		pPointer->increasing = pPointer->increasing * -1.0f;

		pPointer->color = pPointer->nextColor;
		//pPointer->rotationPtr = pPointer->rotationPtr - (pPointer->colorSpeed + pPointer->colorAcceleration);
		pPointer->rotationPtr = pPointer->rotationPtr+ randomf(40.0f,319.9f);
		if(pPointer->rotationPtr > 359.0f) pPointer->rotationPtr = pPointer->rotationPtr - 359.0f;
		pPointer->nextColor = hue_float_to_rgb(pPointer->rotationPtr);
	}
	if(pPointer->acceleration > 0.0) pPointer->acceleration = pPointer->acceleration - 1.0f;
	if(pPointer->colorAcceleration > 0.0) pPointer->colorAcceleration = pPointer->colorAcceleration - 0.5f;
}

void mode_bounce::pointers_tasks(float pStep)
{
	for(size_t i = 0; i < mPointers.size(); i++)
	{
		pointer_tasks(&mPointers[i], pStep);
	}
}

/*
 * public functions
 *
 */
void mode_bounce::paint()
{
	mPointersMutex.lock();
	size_t pointerOffset = 0;
	size_t pointerIndex = 0;

	mLinkedPointers = mSettings["Link All"]->get_value<bool>();
	if(!mLinkedPointers)
	{
		for(size_t i = 0; i < mSize; i++)
		{
			if((i-pointerOffset) >= mPointers[pointerIndex].size)
			{
				pointerOffset += mPointers[pointerIndex].size;
				pointerIndex++;
			}

			size_t pointerPosition = (mPointers[pointerIndex].size-1)*(mPointers[pointerIndex].position) + pointerOffset;
			if(mPointers[pointerIndex].increasing < 0.0)
			{
				if(i <  pointerPosition) mBitmap->set_pixel(i,0,mPointers[pointerIndex].color);
				if(i == pointerPosition) mBitmap->set_pixel(i,0,ColorWhite);
				if(i >  pointerPosition) mBitmap->set_pixel(i,0,mPointers[pointerIndex].nextColor);
			}
			else
			{
				if(i <  pointerPosition) mBitmap->set_pixel(i,0,mPointers[pointerIndex].nextColor);
				if(i == pointerPosition) mBitmap->set_pixel(i,0,ColorWhite);
				if(i >  pointerPosition) mBitmap->set_pixel(i,0,mPointers[pointerIndex].color);
			}
		}
		pointers_tasks(0.004f);
	}
	else
	{
		for(size_t i = 0; i < mSize; i++)
		{
			size_t pointerPosition = (mLinkedPointer.size-1)*(mLinkedPointer.position) + pointerOffset;
			if(mLinkedPointer.increasing < 0.0)
			{
				if(i <  pointerPosition) mBitmap->set_pixel(i,0,mLinkedPointer.color);
				if(i == pointerPosition) mBitmap->set_pixel(i,0,ColorWhite);
				if(i >  pointerPosition) mBitmap->set_pixel(i,0,mLinkedPointer.nextColor);
			}
			else
			{
				if(i <  pointerPosition) mBitmap->set_pixel(i,0,mLinkedPointer.nextColor);
				if(i == pointerPosition) mBitmap->set_pixel(i,0,ColorWhite);
				if(i >  pointerPosition) mBitmap->set_pixel(i,0,mLinkedPointer.color);
			}
		}
		pointer_tasks(&mLinkedPointer, 0.004f);
	}

	mPointersMutex.unlock();
}

void mode_bounce::beat_detected()
{
	if(!mAudioAvailable) return;

	mPointersMutex.lock();

	mSoundReactive = mSettings["Sound Reactive"]->get_value<bool>();
	if(mSoundReactive)
	{
		for(size_t i = 0; i < mPointers.size(); i++)
		{
			mPointers[i].acceleration = 10.0f;
		}
	}

	mPointersMutex.unlock();
}
