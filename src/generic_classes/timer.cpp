/*
 * timer.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "timer.h"

int timerId = 0;

void *global_thread_run(void *owner)
{
	timer *aTimer = (timer *)owner;
	aTimer->thread_run();
	return NULL;
}

/*
 * constructor
 *
 */
timer::timer(long pPeriod, void (*pCallback)(void *pOwner), void *pOwner)
{
	mThreadId = 0;
	mRunning = false;
	mTimerCallback = pCallback;
	mRefreshPeriod = pPeriod;
	mID = ++timerId;
	mOwner = pOwner;
}

timer::~timer()
{
	mRunning = false;
}

void timer::run(void)
{
	mRunning = true;
	(void)pthread_create(&mThreadId, NULL, global_thread_run, this);
}

void timer::update_period(long pPeriod)
{
	mRefreshPeriod = pPeriod;
}

void timer::thread_run()
{
	struct timeval lastRun, now;
	long useconds;

	gettimeofday(&lastRun, NULL);
	while(mRunning)
	{
		gettimeofday(&now, NULL);
		useconds = (now.tv_sec * 1000000 + now.tv_usec) - (lastRun.tv_sec * 1000000 + lastRun.tv_usec);

		if(useconds >= mRefreshPeriod)
		{
			gettimeofday(&lastRun, NULL);
			mTimerCallback(mOwner);
		}
		usleep(100);
	}
}
