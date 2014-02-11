/*
 * mode_off.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#ifndef __MODE_OFF_H__
#define __MODE_OFF_H__

using namespace std;

#include "mode_interface.h"

/*
 * public class
 *
 */
class mode_off : public mode_interface
{
private:
public:
	mode_off(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable);
	~mode_off();

	virtual void paint();
};

#endif
