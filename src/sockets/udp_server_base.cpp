/*
 * udp_server_base.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "udp_server_base.h"

/*
 * constructor
 *
 */
udp_server_base::udp_server_base(boost::asio::io_service& io_service, uint16_t port) : mSocket(io_service, udp::endpoint(udp::v4(), port))
{
	mOwner = NULL;
	mCallback = NULL;
	start_receive();
}

/*
 * destructor
 *
 */
udp_server_base::~udp_server_base()
{

}

/*
 * private functions
 *
 */
void udp_server_base::start_receive()
{
	mSocket.async_receive_from(boost::asio::buffer(mRecvBuffer),
						   mRemoteEndpoint,
						   boost::bind(&udp_server_base::handle_receive,
									   this,
									   boost::asio::placeholders::error,
									   boost::asio::placeholders::bytes_transferred));
}

void udp_server_base::handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	if (!error || error == boost::asio::error::message_size)
	{
		if (mCallback)
		{
			mCallback(mRecvBuffer.begin(), bytes_transferred, mOwner);
		}

		start_receive();
	}
}

void udp_server_base::handle_send(boost::shared_ptr<std::string> message, const boost::system::error_code& error, std::size_t bytes_transferred)
{

}

/*
 * public functions
 *
 */

