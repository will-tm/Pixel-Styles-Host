/*
 * global_event_lopp.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

#include <muduo/net/EventLoop.h>

void initialize_global_event_loop();
muduo::net::EventLoop *get_global_event_loop();
void run_global_event_loop();
