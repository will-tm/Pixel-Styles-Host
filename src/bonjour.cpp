/********************************************************************************************/
/* Copyright (c) 2013 RGB Styles															*/
/********************************************************************************************/

/********************************************************************************************/
/* This file is the confidential trade secret information and/or proprietary information	*/
/* of RGB Styles, Inc. Code or other information in this program also may be confidential	*/
/* and/or proprietary to RGB Styles, Inc.													*/
/* All rights reserved.																		*/
/********************************************************************************************/

/********************************************************************************************/
/* Name : bonjour.cpp																		*/
/* Date : Aug 12 2013																		*/
/* Author : William Markezana																*/
/********************************************************************************************/

/********************************************************************************************/
/* INCLUDES																					*/
/********************************************************************************************/
#include "bonjour.h"

#include <muduo/base/Logging.h>
#include <signal.h>

/********************************************************************************************/
/* PRIVATE VARIABLES																		*/
/********************************************************************************************/

/********************************************************************************************/
/* PRIVATE PROTOTYPES																		*/
/********************************************************************************************/
void *avahi_service(void *arg);

/********************************************************************************************/
/* CONSTRUCTOR																				*/
/********************************************************************************************/
bonjour::bonjour()
{
	mIniFile = new ini_parser("/etc/pixel_styles/config.cfg");
	hostname = mIniFile->get<string>("PIXEL_STYLES","Hostname","Pixel Styles");
	avahi_publish_service_pid = 0;
	mAvahiThread = 0;

	pthread_create(&mAvahiThread, NULL, avahi_service, this);

	delete mIniFile;
}

/********************************************************************************************/
/* DESTRUCTOR																				*/
/********************************************************************************************/
bonjour::~bonjour()
{
	if(kill(avahi_publish_service_pid, SIGINT) == - 1)
	{
		LOG_INFO << "Unable to kill avahi-publish-service process!";
	}

	LOG_INFO << "Closed Service " << hostname;
}

/********************************************************************************************/
/* PRIVATE FUNCTIONS																		*/
/********************************************************************************************/
void *avahi_service(void *arg)
{
	bonjour *mBonjour = (bonjour*)arg;
	pid_t pid;

	pid = fork();
	if (pid <  0) return (void*)(1);
	else if (pid == 0)
	{
		if( execlp("avahi-publish-service", "avahi-publish-service", 	mBonjour->hostname.c_str(),"_PixelStyles._tcp",      "56615", "kHostType=Debian","kHostVersion=7.0 (Wheezy)","kProtocolVersion=2.0","kLivePreviewUdpPort=56616", NULL) == -1 )
		{
			LOG_INFO << "Bad error... couldn't find or failed to run: avahi-publish-service OR dns-sd OR mDNSPublish";
			return (void*)(1);
		}
	}

	LOG_INFO << "Started avahi-publish-service with pid " << pid << " hostname '" << mBonjour->hostname << "'";
	mBonjour->avahi_publish_service_pid = pid;

	return (void*)(1);
}

/********************************************************************************************/
/* END OF FILE																				*/
/********************************************************************************************/
