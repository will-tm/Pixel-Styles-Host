/*
 * mode_tunnel.h
 *
 * Copyright (C) 2016 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

using namespace std;

#include "mode_interface.h"

/*
 * public class
 *
 */
class mode_tunnel: public mode_interface
{
private:
	/*
	 * private typedefs
	 *
	 */
	struct space_ship_t {
		point_t position;
		rgb_color color;
		int score;
	};

	struct wall_t {
		int position;
		int width;
	};

	bool mFirstLaunch;
	uint32_t mLastRun;
	bool mLost;
	vector<struct wall_t> mTunnel;
	space_ship_t mSpaceShip;

	void initialize_tunnel();
	void move_tunnel();
	void collision_detection();
public:
	mode_tunnel(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable, vector<size_t> pSegments);
	~mode_tunnel();

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
