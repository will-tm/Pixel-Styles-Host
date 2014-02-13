/*
 * mode_udp_streamer.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#ifndef __MODE_UDP_STREAMER_H__
#define __MODE_UDP_STREAMER_H__

using namespace std;

#include "mode_interface.h"
#include "udp_server.h"

/*
 * public class
 *
 */
class mode_udp_streamer : public mode_interface
{
private:
	udp_server *mUdpServer;
	uint8_t *mUdpBuffer;
	uint32_t mLastTickCount;

	//void udp_callback(uint8_t *data, size_t length);
	void handle_receive(data_packet_t packet);
public:
	mode_udp_streamer(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable);
	~mode_udp_streamer();

	virtual void paint();
};

#endif
