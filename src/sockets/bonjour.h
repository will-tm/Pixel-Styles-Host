/*
 * bonjour.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#ifndef __BONJOUR_H__
#define __BONJOUR_H__

using namespace std;

#include <string>
#include "ini_parser.h"
#include "tcp_server.h"

/*
 * public class
 *
 */
class bonjour
{
private:
	ini_parser *mIniFile;
	pthread_t mAvahiThread;
public:
	string hostname;
	pid_t avahi_publish_service_pid;

	bonjour();
	~bonjour();
};

#endif
