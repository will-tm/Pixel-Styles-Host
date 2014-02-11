/*
 * timer.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <cstdio>

using namespace std;

/*
 * public class
 *
 */
class timer
{
private:
	bool mRunning;
	pthread_t mThreadId;
	long mRefreshPeriod;
	void (*mTimerCallback)(void *pOwner);
	void *mOwner;
	int mID;
public:
	timer(long Period, void (*Callback)(void *Owner), void *pOwner);
	virtual ~timer();

	void run(void);
	void update_period(long Period);
	void thread_run();
};

#endif
