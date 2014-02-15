/*
 * bonjour.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "bonjour.h"

#include <boost/lexical_cast.hpp>
#include <muduo/base/Logging.h>
#include <signal.h>

void *avahi_service(void *arg);

/*
 * constructor
 *
 */
bonjour::bonjour()
{
	mIniFile = new ini_parser((string) CONFIGURATION_DIRECTORY + "config.cfg");
	hostname = mIniFile->get<string>("PIXEL_STYLES", "Hostname",
			"Pixel Styles");
	avahi_publish_service_pid = 0;
	mAvahiThread = 0;
	
	pthread_create(&mAvahiThread, NULL, avahi_service, this);
	
	delete mIniFile;
}

/*
 * destructor
 *
 */
bonjour::~bonjour()
{
	if (kill(avahi_publish_service_pid, SIGINT) == -1)
	{
		LOG_INFO << "Unable to kill avahi-publish-service process!";
	}
	
	LOG_INFO << "Closed Service " << hostname;
}

/*
 * private callbacks
 *
 */
void *avahi_service(void *arg)
{
	bonjour *mBonjour = (bonjour*) arg;
	pid_t pid;
	
	pid = fork();
	if (pid < 0)
		return (void*) (1);
	else if (pid == 0)
	{
		if (execlp("avahi-publish-service", "avahi-publish-service",
				mBonjour->hostname.c_str(), "_PixelStyles._tcp",
				boost::lexical_cast<string>(TCP_CONNECTION_PORT).c_str(),
				strcat((char*) "kLivePreviewUdpPort=",
						boost::lexical_cast<string>(UDP_BROADCAST_PORT).c_str()),
				NULL) == -1)
		{
			LOG_INFO
					<< "Bad error... couldn't find or failed to run: avahi-publish-service OR dns-sd OR mDNSPublish";
			return (void*) (1);
		}
	}
	
	LOG_INFO << "Started avahi-publish-service with pid " << pid
			<< " hostname '" << mBonjour->hostname << "'";
	mBonjour->avahi_publish_service_pid = pid;
	
	return (void*) (1);
}
