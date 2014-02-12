/*
 * global_event_lopp.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#ifndef __GLOBAL8EVENT_LOOP_H__
#define __GLOBAL8EVENT_LOOP_H__

#include <muduo/net/EventLoop.h>

muduo::net::EventLoop *get_global_event_loop();
void run_global_event_loop();

#endif
