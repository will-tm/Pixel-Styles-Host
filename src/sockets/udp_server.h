/*
 * udp_server.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#ifndef __UDP_SERVER_H__
#define __UDP_SERVER_H__

using namespace std;


#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "splitter.h"
#include "timer.h"

/*
 * public types
 *
 */
typedef void (udp_socket_callback_t)(uint8_t *data, int length, void *owner);

/*
 * public class
 *
 */
class udp_server
{
private:
	timer *mTimer;
	uint16_t mPort;
	uint8_t mBuffer[8192];
	int mSocket;
	struct sockaddr_in mSocketAddrMe;
	struct sockaddr_in mSocketAddrClient;
	socklen_t mSocketLength;
	udp_socket_callback_t *mCallback;
	void *mOwner;
public:
	udp_server(uint16_t pPort);
	~udp_server();

	void run(void);
	void periodic_tasks();

	void register_callback(udp_socket_callback_t *callback, void *owner) { mCallback = callback; mOwner = owner; }
};

/********************************************************************************************/
/* END OF FILE																				*/
/********************************************************************************************/
#endif
