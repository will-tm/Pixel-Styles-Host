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

/*
 * public types
 *
 */
typedef void (TcpReceptionCallback)(string __request, string &__answer, int __clientId, void* pParent);

/*
 * public class
 *
 */
class tcp_server
{
private:
	void onConnection(const muduo::net::TcpConnectionPtr& conn);
	void onMessage(const muduo::net::TcpConnectionPtr& conn,muduo::net::Buffer* buf,muduo::Timestamp time);
	muduo::net::EventLoop* loop_;
	muduo::net::TcpServer server_;
	int numConnected_; // should be atomic_int
	const int kMaxConnections_;
	void* mCallbackParent;
	vector<string> mConnectedAddresses;
	string mMacAddress;
public:
	TcpReceptionCallback *tcpReceptionCallback;

	tcp_server(muduo::net::EventLoop* loop, const muduo::net::InetAddress& listenAddr, int maxConnections);
	void registerCallback(TcpReceptionCallback *callback, void* pParent) { tcpReceptionCallback = callback; mCallbackParent = pParent; }
	void run(void);
	string mac_address();
	vector<string> *connectedAddresses();
};

#endif
