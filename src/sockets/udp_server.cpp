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
udp_server::udp_server(uint16_t pPort)
{
	mUdpServerBase = new udp_server_base(mIoService, pPort);
}

/*
 * destructor
 *
 */
udp_server::~udp_server()
{

}

/*
 * public functions
 *
 */
void udp_server::run(void)
{
	//mIoService.run();
}
