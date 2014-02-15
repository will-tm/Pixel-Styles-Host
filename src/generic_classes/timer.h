/*
 * timer.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

#include <time.h>
#include <iostream>
#include <signal.h>
#include <cstring>
#include <functional>

using namespace std;

/*
 * public types
 *
 */
typedef function<void()> timer_callback_t;

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
    timer_t mTimer;
    timer_callback_t mCallback;
public:
	timer(long pPeriod, const timer_callback_t& callback);
	virtual ~timer();

    static void handler_wrapper(sigval_t val);
    void handler();

	void run(void);
};
