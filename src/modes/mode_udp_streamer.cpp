/*
 * mode_udp_streamer.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_udp_streamer.h"

static void udp_callback(uint8_t *data, size_t length, void *owner);

#define UDP_TIMEOUT_DELAY					(2000000) // us

/*
 * constructor
 *
 */
mode_udp_streamer::mode_udp_streamer(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable) : mode_interface(pWidth, pHeight, pName, pAudioAvailable)
{
	mLastTickCount = 0;

	mUdpBuffer = new uint8_t [mWidth * mHeight * 3];

    mUdpServer = new udp_server(get_global_event_loop(), 58618);
    mUdpServer->register_callback(udp_callback, (void*)this);
}

/*
 * destructor
 *
 */
mode_udp_streamer::~mode_udp_streamer()
{

}

/*
 * private callbacks
 *
 */
static void udp_callback(uint8_t *data, size_t length, void *owner)
{
	mode_udp_streamer *modeUdpStreamer = (mode_udp_streamer*)owner;
	if (length == modeUdpStreamer->get_width() * modeUdpStreamer->get_height() * 3)
	{
		memcpy(modeUdpStreamer->get_udp_buffer(), data, length);
	}
}

/*
 * public functions
 *
 */
void mode_udp_streamer::paint()
{
	rgb_color color;
	int bufferPtr = 0;

	if (get_tick_us() - mLastTickCount < UDP_TIMEOUT_DELAY)
	{
		mLastTickCount = get_tick_us();

		for(size_t y = 0; y < mHeight; y++)
		{
			for(size_t x = 0; x < mWidth; x++)
			{
				color.R = mUdpBuffer[bufferPtr++];
				color.G = mUdpBuffer[bufferPtr++];
				color.B = mUdpBuffer[bufferPtr++];
				mBitmap->set_pixel(x,y,color);
			}
		}
	}
	else
	{
		mBitmap->clear();
	}
}
