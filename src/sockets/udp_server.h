/*
 * udp_server.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#ifndef __UDP_SERVER_H__
#define __UDP_SERVER_H__

using namespace std;


#include "udp_server_base.h"

/*
 * public class
 *
 */
class udp_server
{
private:
	boost::asio::io_service mIoService;
	udp_server_base *mUdpServerBase;
public:
	udp_server(uint16_t pPort);
	~udp_server();

	void run(void);

	void register_callback(udp_socket_callback_t *callback, void *owner) { mUdpServerBase->register_callback(callback, owner); }
};

/********************************************************************************************/
/* END OF FILE																				*/
/********************************************************************************************/
#endif
