/*
 * mode_udp_streamer.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

using namespace std;

#include "mode_interface.h"

/*
 * public class
 *
 */
class mode_udp_streamer: public mode_interface
{
private:
	uint8_t *mUdpBuffer;
	uint32_t mLastTickCount;

public:
	mode_udp_streamer(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable);
	~mode_udp_streamer();

	virtual void paint();
	virtual void handle_udp_receive(uint8_t *data, size_t length);

	virtual bool needs_udp_socket()
	{
		return true;
	}

	virtual uint16_t udp_port()
	{
		return 58618;
	}
};
