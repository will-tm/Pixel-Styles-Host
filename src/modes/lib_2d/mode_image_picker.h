/*
 * mode_image_picker.h
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
class mode_image_picker: public mode_interface
{
private:
	uint8_t *mUdpBuffer;

public:
	mode_image_picker(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable);
	~mode_image_picker();

	virtual void paint();
	virtual void handle_udp_receive(uint8_t *pData, size_t pLength);

	virtual bool needs_udp_socket()
	{
		return true;
	}

	virtual uint16_t udp_port()
	{
		return 58619;
	}
};
