/*
 * global_event_lopp.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "global_event_loop.h"
#include <muduo/base/Logging.h>

muduo::net::EventLoop loop;

muduo::net::EventLoop *get_global_event_loop()
{
	return &loop;
}

void run_global_event_loop()
{
	LOG_INFO << "main loop start running...";
	loop.loop();
}
