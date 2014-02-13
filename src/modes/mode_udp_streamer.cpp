/*
 * mode_udp_streamer.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_udp_streamer.h"

#include <muduo/base/Logging.h>

#define UDP_TIMEOUT_DELAY					(2000000) // us

/*
 * constructor
 *
 */
mode_udp_streamer::mode_udp_streamer(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable) : mode_interface(pWidth, pHeight, pName, pAudioAvailable)
{
	mLastTickCount = 0;

	mUdpBuffer = new uint8_t [mWidth * mHeight * 3];

    mUdpServer = new udp_server(get_global_event_loop(), MODE_UDP_STREAMER_PORT);
    mUdpServer->register_read_callback(boost::bind(&mode_udp_streamer::handle_receive, this, _1));
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
void mode_udp_streamer::handle_receive(udp_data_packet_t packet)
{
	if (packet.length == mWidth * mHeight * 3)
	{
		mLastTickCount = get_tick_us();
		memcpy(mUdpBuffer, packet.data, packet.length);
	}
	else
	{
		LOG_ERROR << "Erroneous length on mode_udp_streamer (" << packet.length << " <> " << (mWidth * mHeight * 3) << ")";
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
