/*
 * tcp_server.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

using namespace std;

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdio.h>
#include <string.h>
#include <muduo/net/TcpServer.h>
#include <string>
#include <vector>
#include "splitter.h"

#include <boost/bind.hpp>
#include <functional>

/*
 * public types
 *
 */
typedef function<void(string request, string &answer)> tcp_socket_read_callback_t;

/*
 * public class
 *
 */
class tcp_server
{
private:
	void onConnection(const muduo::net::TcpConnectionPtr& conn);
	void onMessage(const muduo::net::TcpConnectionPtr& conn,
			muduo::net::Buffer* buf, muduo::Timestamp time);
	muduo::net::EventLoop* loop_;
	muduo::net::TcpServer server_;
	int numConnected_; // should be atomic_int
	const int kMaxConnections_;
	void* mCallbackParent;
	vector<string> mConnectedAddresses;
	string mMacAddress;
	tcp_socket_read_callback_t mReadCallback;
public:
	tcp_server(muduo::net::EventLoop* loop,
			const muduo::net::InetAddress& listenAddr, int maxConnections);

	void run(void);
	string mac_address();
	vector<string> *connectedAddresses();

	void register_read_callback(const tcp_socket_read_callback_t& callback)
	{
		mReadCallback = callback;
	}
};

#endif
