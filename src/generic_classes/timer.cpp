/*
 * timer.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "timer.h"

#include <muduo/base/Logging.h>

/*
 * constructor
 *
 */
timer::timer(long pPeriod, const timer_callback_t& callback)
{
	mThreadId = 0;
	mRunning = false;
	mRefreshPeriod = pPeriod;
	mCallback = callback;
	mTimer = 0;
}

/*
 * destructor
 *
 */
timer::~timer()
{
	mRunning = false;
}

/*
 * public functions
 *
 */
void timer::run(void)
{
	struct sigevent sigev;

	memset(&sigev, 0, sizeof (struct sigevent));
	sigev.sigev_value.sival_int = 0;
	sigev.sigev_notify = SIGEV_THREAD;
	sigev.sigev_notify_attributes = NULL;
	sigev.sigev_value.sival_ptr = (void*)this;
	sigev.sigev_notify_function = &timer::handler_wrapper;

	if(timer_create(CLOCK_REALTIME, &sigev, &mTimer) == 0)
	{
		struct itimerspec itval;
		struct itimerspec oitval;

		itval.it_value.tv_sec = mRefreshPeriod / 1000000;
		itval.it_value.tv_nsec = (long)(mRefreshPeriod % 1000000) * (1000L);

		itval.it_interval.tv_sec = itval.it_value.tv_sec;
		itval.it_interval.tv_nsec = itval.it_value.tv_nsec;

		if(timer_settime(mTimer, 0, &itval, &oitval) != 0)
		{
			LOG_ERROR << "time_settime error!";
			return;
		}
	}
	else
	{
		LOG_ERROR << "timer_create error!";
		return;
	}
}

void timer::handler_wrapper(sigval_t val)
{
	timer *aTimer = (timer *)val.sival_ptr;
	aTimer->handler();
}

void timer::handler()
{
	if(mCallback) mCallback();
}
