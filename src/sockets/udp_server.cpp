/*
 * udp_server.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "udp_server.h"

/*
 * constructor
 *
 */
udp_server::udp_server(muduo::net::EventLoop* loop, uint16_t pPort)
{
	mPort = pPort;

	mUdpSocket = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, IPPROTO_UDP);
	if (mUdpSocket < 0)
	{
		LOG_SYSFATAL << "::socket";
	}
	else
	{
		mSocket = new muduo::net::Socket(mUdpSocket);
		mSocket->bindAddress(muduo::net::InetAddress(mPort));

		mChannel = new muduo::net::Channel(loop, mSocket->fd());
		mChannel->setReadCallback(boost::bind(&udp_server::handle_receive, this, mSocket->fd(), _1));
		mChannel->enableReading();
	}
}

/*
 * destructor
 *
 */
udp_server::~udp_server()
{
	mChannel->disableAll();
	delete mChannel;
	delete mSocket;
}

/*
 * private functions
 *
 */
void udp_server::handle_receive(int sockfd, muduo::Timestamp receiveTime)
{
	struct sockaddr peerAddr;
	bzero(&peerAddr, sizeof peerAddr);
	socklen_t addrLen = sizeof peerAddr;
	size_t length = ::recvfrom(sockfd, mBuffer, 8192, 0, &peerAddr, &addrLen);

	char addrStr[32];
	muduo::net::sockets::toIpPort(addrStr, sizeof addrStr, *reinterpret_cast<struct sockaddr_in*>(&peerAddr));
	LOG_DEBUG << "received " << length << " bytes from " << addrStr;

	if (length >= 0)
	{
		if(mReadCallback) mReadCallback({mBuffer, length});
	}
}
