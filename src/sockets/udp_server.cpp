/*
 * udp_server.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "udp_server.h"

using namespace muduo;
using namespace muduo::net;

const size_t frameLen = 2*sizeof(int64_t);

/*
 * constructor
 *
 */
udp_server::udp_server(muduo::net::EventLoop* loop, uint16_t pPort)
{
	mOwner = NULL;
	mCallback = NULL;

	int sockfd = ::socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_UDP);
	if (sockfd < 0)
	{
		LOG_SYSFATAL << "::socket";
	}
	else
	{
		Socket sock(sockfd);
		sock.bindAddress(InetAddress(pPort));

		Channel channel(loop, sock.fd());
		channel.setReadCallback(boost::bind(&udp_server::handle_receive, this, sock.fd(), _1));
		channel.enableReading();
	}
}

/*
 * destructor
 *
 */
udp_server::~udp_server()
{

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
	ssize_t length = ::recvfrom(sockfd, mBuffer, 8192, 0, &peerAddr, &addrLen);

	char addrStr[32];
	sockets::toIpPort(addrStr, sizeof addrStr, *reinterpret_cast<struct sockaddr_in*>(&peerAddr));
	LOG_DEBUG << "received " << length << " bytes from " << addrStr;

	/*
	if (length < 0)
	{
		LOG_SYSERR << "::recvfrom";
	}
	else if (implicit_cast<size_t>(nr) == frameLen)
	{
		message[1] = receiveTime.microSecondsSinceEpoch();
		ssize_t nw = ::sendto(sockfd, message, sizeof message, 0, &peerAddr, addrLen);
		if (nw < 0)
		{
			LOG_SYSERR << "::sendto";
		}
		else if (implicit_cast<size_t>(nw) != frameLen)
		{
			LOG_ERROR << "Expect " << frameLen << " bytes, wrote " << nw << " bytes.";
		}
	}
	else
	{
		LOG_ERROR << "Expect " << frameLen << " bytes, received " << nr << " bytes.";
	}*/

	if (length >= 0)
	{
		if(mCallback)
		{
			mCallback(mBuffer, length, mOwner);
		}
	}
}
