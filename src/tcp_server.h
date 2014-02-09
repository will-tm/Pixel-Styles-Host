/********************************************************************************************/
/* Copyright (c) 2012 RGB Styles															*/
/********************************************************************************************/

/********************************************************************************************/
/* This file is the confidential trade secret information and/or proprietary information	*/
/* of RGB Styles, Inc. Code or other information in this program also may be confidential	*/
/* and/or proprietary to RGB Styles, Inc.													*/
/* All rights reserved.																		*/
/********************************************************************************************/

/********************************************************************************************/
/* Name : tcp_server.cpp																	*/
/* Date : Jan 25 2012																		*/
/* Author : William Markezana																*/
/********************************************************************************************/

#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

using namespace std;

/********************************************************************************************/
/* INCLUDES																					*/
/********************************************************************************************/
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

/********************************************************************************************/
/* DEFINES																					*/
/********************************************************************************************/

/********************************************************************************************/
/* TYPEDEFS																					*/
/********************************************************************************************/
typedef void (TcpReceptionCallback)(string __request, string &__answer, int __clientId, void* pParent);

/********************************************************************************************/
/* CLASSES																					*/
/********************************************************************************************/
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

/********************************************************************************************/
/* END OF FILE																				*/
/********************************************************************************************/
#endif
