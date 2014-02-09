/********************************************************************************************/
/* Copyright (c) 2013 RGB Styles                              								*/
/********************************************************************************************/

/********************************************************************************************/
/* This file is the confidential trade secret information and/or proprietary information  	*/
/* of RGB Styles, Inc. Code or other information in this program also may be confidential 	*/
/* and/or proprietary to RGB Styles, Inc.                         							*/
/* All rights reserved.                                   									*/
/********************************************************************************************/

/********************************************************************************************/
/* Name : timer.h		                                 									*/
/* Date : Oct 21 2013                                   									*/
/* Author : William Markezana                               								*/
/********************************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

/********************************************************************************************/
/* INCLUDES                                         										*/
/********************************************************************************************/
#include <time.h>
#include <iostream>
#include <signal.h>
#include <cstring>

using namespace std;

/********************************************************************************************/
/* DEFINES	                                         										*/
/********************************************************************************************/
#define CLOCKS_PER_MSEC (CLOCKS_PER_SEC/1000)

/********************************************************************************************/
/* CLASSES	                                         										*/
/********************************************************************************************/
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
	void update_period(long pPeriod);
};

/********************************************************************************************/
/* END OF FILE																				*/
/********************************************************************************************/
#endif
