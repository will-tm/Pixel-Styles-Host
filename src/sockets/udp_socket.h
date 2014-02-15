/*
 * udp_socket.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#ifndef __UDP_SOCKET_H__
#define __UDP_SOCKET_H__

using namespace std;

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include "splitter.h"

/*
 * public class
 *
 */
class udp_socket
{
private:
	int mSock;
	int mBroadcastSock;
	struct sockaddr_in mBroadcastAddress;
public:
	udp_socket(uint16_t port);
	~udp_socket();

	void run(void);
	void broadcastMessage(string pMessage);
	void sendMessageToHost(string pMessage, string pHost, uint16_t pPort);
};

#endif
