/*
 * mode_bounce.h
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
	float position;
	float increasing;
	rgb_color color;
	rgb_color nextColor;
	float rotationPtr;
	float speed;
	float acceleration;
	float colorSpeed;
	float colorAcceleration;
	size_t size;
}mode_bounce_pointer;

/*
 * public class
 *
 */
class mode_bounce : public mode_interface
{
private:
	vector<mode_bounce_pointer> mPointers;
	mode_bounce_pointer mLinkedPointer;
	mutex mPointersMutex;
	bool mLinkedPointers;

	void pointers_tasks(float step);
	void configure_pointer(mode_bounce_pointer *pPointer, size_t pSize);
	void pointer_tasks(mode_bounce_pointer *pPointer, float pStep);
public:
	mode_bounce(size_t pSize, string pName, bool pAudioAvailable, vector<size_t> pSegments);
	~mode_bounce();

	virtual void paint();
	virtual void beat_detected();
};
