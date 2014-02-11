/*
 * udp_socket.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "udp_socket.h"

#include <muduo/base/Logging.h>
#include "zlib.h"

/*
 * constructor
 *
 */
udp_socket::udp_socket()
{
	mSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	mBroadcastSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	/*
	 * SO_BROADCAST doesn't sent to all NICs on Linux, it only
	 * broadcasts on eth0, which is why there's an optional argument
	 * to --broadcast to say which NIC to broadcast on. You can use
	 * SO_BINDTODEVICE to get around that, but you need to have
	 * uid == 0 for that
	 */
	int on = 1;
	if(setsockopt(mBroadcastSock, SOL_SOCKET, SO_BROADCAST, (int *)&on, sizeof(on)) < 0)
	{
		perror("setsockopt");
	}
	//shutdown(mBroadcastSock, SHUT_RD);

	memset(&mBroadcastAddress, '\0', sizeof(struct sockaddr_in));
	mBroadcastAddress.sin_family = AF_INET;
	mBroadcastAddress.sin_port = (in_port_t)htons(UDP_PORT);
	mBroadcastAddress.sin_addr.s_addr = htonl(INADDR_BROADCAST);
}

/*
 * destructor
 *
 */
udp_socket::~udp_socket()
{
	close(mSock);
	close(mBroadcastSock);
}

/*
 * public functions
 *
 */
void udp_socket::run(void)
{

}

void udp_socket::broadcastMessage(string pMessage)
{
	unsigned int sourceSize = pMessage.size();
	const char * source = pMessage.c_str();
	unsigned long dsize = sourceSize + (sourceSize * 0.1f) + 16;
	char * destination = new char[dsize];
	int result = compress2((unsigned char *)destination, &dsize, (const unsigned char *)source, sourceSize, Z_DEFAULT_COMPRESSION);
	if(result != Z_OK) LOG_ERROR << "Compress error occured! Error code: " << result;

	if(sendto(mBroadcastSock, destination, dsize, 0, (struct sockaddr *)&mBroadcastAddress, sizeof(struct sockaddr_in)) < 0)
		LOG_ERROR << "Unable to broadcast message '" << pMessage << "'";

	delete [] destination;
}

void udp_socket::sendMessageToHost(string pMessage, string pHost, uint16_t pPort)
{
	unsigned int sourceSize = pMessage.size();
	const char * source = pMessage.c_str();
	unsigned long dsize = sourceSize + (sourceSize * 0.1f) + 16;
	char * destination = new char[dsize];
	int result = compress2((unsigned char *)destination, &dsize, (const unsigned char *)source, sourceSize, Z_DEFAULT_COMPRESSION);
	if(result != Z_OK) LOG_ERROR << "Compress error occured! Error code: " << result;

	struct sockaddr_in si_other;
	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(pPort);
	if (inet_aton(pHost.c_str(), &si_other.sin_addr)==0)
	{
		LOG_ERROR << "inet_aton() failed\n";
		return;
	}
	if (sendto(mSock, destination, dsize, 0, (struct sockaddr *)&si_other, sizeof(struct sockaddr_in)) < 0)
		LOG_ERROR << "Unable to send message '" << destination << "' to '" << pHost << "' on port" << pPort;

	delete [] destination;
}
