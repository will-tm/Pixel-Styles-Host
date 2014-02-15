/*
 * udp_server.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#ifndef __UDP_SERVER_H__
#define __UDP_SERVER_H__

using namespace std;

#include <muduo/base/Logging.h>
#include <muduo/net/Channel.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/Socket.h>
#include <muduo/net/SocketsOps.h>
#include <muduo/net/TcpClient.h>
#include <muduo/net/TcpServer.h>

#include <boost/bind.hpp>
#include <functional>

/*
 * public types
 *
 */
typedef function<void(uint8_t *data, size_t length)> udp_socket_read_callback_t;

/*
 * public class
 *
 */
class udp_server
{
private:
	int mUdpSocket;
	muduo::net::Socket *mSocket;
	muduo::net::Channel *mChannel;
	uint16_t mPort;
	uint8_t mBuffer[8192];
	udp_socket_read_callback_t mReadCallback;

	void handle_receive(int sockfd, muduo::Timestamp receiveTime);
public:
	udp_server(muduo::net::EventLoop* loop, uint16_t pPort);
	~udp_server();

	void register_read_callback(const udp_socket_read_callback_t& callback)
	{
		mReadCallback = callback;
	}
};

/********************************************************************************************/
/* END OF FILE																				*/
/********************************************************************************************/
#endif
