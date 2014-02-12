/*
 * udp_server_base.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#ifndef __UDP_SERVER_BASE_H__
#define __UDP_SERVER_BASE_H__

#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

/*
 * public types
 *
 */
typedef void (udp_socket_callback_t)(uint8_t *data, size_t length, void *owner);

/*
 * public class
 *
 */
class udp_server_base
{
private:
	udp::socket mSocket;
	udp::endpoint mRemoteEndpoint;
	boost::array<uint8_t, 1> mRecvBuffer;

	udp_socket_callback_t *mCallback;
	void *mOwner;

	void start_receive();
	void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred);
	void handle_send(boost::shared_ptr<std::string> message, const boost::system::error_code& error, std::size_t bytes_transferred);
public:
	udp_server_base(boost::asio::io_service& io_service, uint16_t port);
	~udp_server_base();

	void register_callback(udp_socket_callback_t *callback, void *owner) { mCallback = callback; mOwner = owner; }
};

#endif
