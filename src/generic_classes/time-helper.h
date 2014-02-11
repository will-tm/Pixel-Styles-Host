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
inline long getCurrentTime() // micro-seconds
{
	struct timeval Now;
	gettimeofday(&Now, NULL);
	long useconds = Now.tv_sec * 1000000 + Now.tv_usec;
	return useconds;
}
