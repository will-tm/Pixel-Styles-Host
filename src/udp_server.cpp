/*
 * udp_server.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "udp_server.h"

static void periodic_callback(void *owner);
static void diep(const char *s);

/*
 * constructor
 *
 */
udp_server::udp_server(uint16_t pPort)
{
	mTimer = new timer(10000, periodic_callback, this);
	mCallback = NULL;
	mOwner = NULL;
	mPort = pPort;

	mSocketLength = sizeof(mSocketAddrClient);

	if ((mSocket=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
		diep("socket");

	memset((char *) &mSocketAddrMe, 0, sizeof(mSocketAddrMe));
	mSocketAddrMe.sin_family = AF_INET;
	mSocketAddrMe.sin_port = htons(mPort);
	mSocketAddrMe.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(mSocket, (const sockaddr*)&mSocketAddrMe, sizeof(mSocketAddrMe))==-1)
		diep("bind");
}

/*
 * destructor
 *
 */
udp_server::~udp_server()
{
	delete mTimer;
}

/*
 * private callbacks
 *
 */
static void periodic_callback(void *owner)
{
	udp_server *server = (udp_server*)owner;
	server->periodic_tasks();
}

void diep(const char *s)
{
	perror(s);
	exit(1);
}

/*
 * public functions
 *
 */
void udp_server::run(void)
{
	mTimer->run();
}

void udp_server::periodic_tasks()
{
	int length = recvfrom(mSocket, mBuffer, 8192, 0, (sockaddr*)&mSocketAddrClient, &mSocketLength);
	if (length >= 0)
	{
		//printf("Received packet of length %d from %s:%d\nData: %s\n\n",length,inet_ntoa(mSocketAddrClient.sin_addr), ntohs(mSocketAddrClient.sin_port), mBuffer);
		if(mCallback != NULL)
		{
			mCallback(mBuffer, length, mOwner);
		}
	}
}
