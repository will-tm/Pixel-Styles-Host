/*
 * timer.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <time.h>
#include <iostream>
#include <signal.h>
#include <cstring>

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
	void (*mTimerCallback)(void* pParent);
	void *mCallbackParent;
    timer_t mTimer;
public:
	timer(long pPeriod, void (*Callback)(void* pParent), void* pParent);
	virtual ~timer();

    static void handler_wrapper(sigval_t val);
    void handler();

	void run(void);
};

#endif
