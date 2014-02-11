/*
 * time-helper.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

#include <time.h>
#include <sys/time.h>

using namespace std;

/*
 * public functions
 *
 */
inline uint32_t get_tick_us()
{
	struct timeval Now;
	uint32_t tick;

	gettimeofday(&Now, NULL);
	tick = (Now.tv_sec * 1000000 + Now.tv_usec);
	return tick;
}
