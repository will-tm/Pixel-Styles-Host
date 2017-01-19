/*
 * mode_tunnel.cpp
 *
 * Copyright (C) 2016 William Markezana <william.markezana@me.com>
 *
 */

#include "mode_tunnel.h"


static const rgb_color space_ship_color = { 0x00, 0xff, 0x00, 0xff };

/*
 * public library interface
 *
 */
extern "C" mode_interface* create_mode(size_t pWidth, size_t pHeight, bool pAudioAvailable, vector<size_t> pSegments)
{
  return new mode_tunnel(pWidth, pHeight, "Tunnel", pAudioAvailable, pSegments);
}

extern "C" void destroy_mode(mode_interface* object)
{
  delete object;
}

/*
 * constructor
 *
 */
mode_tunnel::mode_tunnel(size_t pWidth, size_t pHeight, string pName, bool pAudioAvailable, vector<size_t> pSegments)
: mode_interface(pWidth, pHeight, pName, pAudioAvailable, pSegments)
{	
	mFirstLaunch = true;
	mLastRun = 0;
	mLost = false;
	mTunnel.resize(mHeight);
}

/*
 * destructor
 *
 */
mode_tunnel::~mode_tunnel()
{
	
}

/*
 * private functions
 *
 */
void mode_tunnel::initialize_tunnel()
{
	mSpaceShip.score = 0;
	mSpaceShip.position.x = mWidth / 2;
	mSpaceShip.position.y = mHeight - 1;
	mSpaceShip.color = space_ship_color;

	for (size_t i = 0; i < mTunnel.size(); i++) {
		mTunnel[i].position = 4;
		mTunnel[i].width = 10;
	}
	mLost = false;
}

void mode_tunnel::move_tunnel()
{
	bool ready = false;
	int difficulty = mSpaceShip.score / 500;
	struct wall_t new_wall;

	do {
		new_wall.width = 10 - difficulty + (mSpaceShip.score < 200 ? 1 : 0);
		new_wall.position = mTunnel[0].position + random(3) - 1;
		ready = ((new_wall.position + new_wall.width) <  mWidth &&  new_wall.position >= 0);
	} while (not ready);

	for (size_t i = mTunnel.size() - 1; i >= 1; i--) {
		mTunnel[i].position = mTunnel[i-1].position;
		mTunnel[i].width = mTunnel[i-1].width;
	}
	mTunnel[0] = new_wall;
}

void mode_tunnel::collision_detection()
{
	if (mSpaceShip.position.x-1 == mTunnel[mSpaceShip.position.y].position) mLost = true;
	if (mSpaceShip.position.x   == mTunnel[mSpaceShip.position.y].position + mTunnel[mSpaceShip.position.y].width) mLost = true;
}

/*
 * public functions
 *
 */
void mode_tunnel::paint()
{
	int speed;
	rgb_color tunnel_color = int_to_rgb(0xffff0000);

	speed = (45 - mSpaceShip.score / 500);
	if (speed < 15) speed = 15;

	if (mFirstLaunch) {
		initialize_tunnel();
		mFirstLaunch = false;
	}

	if ((get_tick_us() - mLastRun) > speed * 1000 && not mLost) {
		mLastRun = get_tick_us();

		move_tunnel();
		collision_detection();
		mSpaceShip.score++;			
	}

	if (not mLost) {
		//draw
		mBitmap->clear();

		// draw tunnel
		for (size_t y = 0; y < mTunnel.size(); y++) {
			// adjust color to score
			if (mSpaceShip.score > 500)  tunnel_color = int_to_rgb(0xffff8000);
			if (mSpaceShip.score > 1000) tunnel_color = int_to_rgb(0xff00ff00);
			if (mSpaceShip.score > 1500) tunnel_color = int_to_rgb(0xff00ff80);
			if (mSpaceShip.score > 2000) tunnel_color = int_to_rgb(0xff00ffff);
			if (mSpaceShip.score > 2500) tunnel_color = int_to_rgb(0xff0080ff);
			if (mSpaceShip.score > 3000) tunnel_color = int_to_rgb(0xff0000ff);
			if (mSpaceShip.score > 3500) tunnel_color = random_rgb();

			for (int x = 0; x <= mTunnel[y].position; x++) mBitmap->set_pixel(x, y, tunnel_color);
			for (int x = mTunnel[y].position + mTunnel[y].width + 1; x < mWidth; x++) mBitmap->set_pixel(x, y, tunnel_color);
		}

		// draw space ship
		mBitmap->set_pixel(mSpaceShip.position.x  , mSpaceShip.position.y  , mSpaceShip.color);
		mBitmap->set_pixel(mSpaceShip.position.x+1, mSpaceShip.position.y  , mSpaceShip.color);
		mBitmap->set_pixel(mSpaceShip.position.x-1, mSpaceShip.position.y  , mSpaceShip.color);
		mBitmap->set_pixel(mSpaceShip.position.x  , mSpaceShip.position.y-1, mSpaceShip.color);
	} else {
		mBitmap->fill(ColorRed);
		if ((get_tick_us() - mLastRun) >= 3000000) {
			mFirstLaunch = true;
		}
	}
}

void mode_tunnel::handle_udp_receive(uint8_t *pData, size_t pLength)
{
	string command((char*)pData, pLength);
	printf("%s\n",command.c_str());

	if (command.compare("up") == 0) {
 		mSpaceShip.position.y = MAX(--mSpaceShip.position.y, 0);
	} else if (command.compare("down") == 0) {
 		mSpaceShip.position.y = MIN(++mSpaceShip.position.y, mHeight-1);
	} else if (command.compare("left") == 0) {
 		mSpaceShip.position.x = MAX(mSpaceShip.position.x, 0);
	} else if (command.compare("right") == 0) {
 		mSpaceShip.position.x = MIN(mSpaceShip.position.x, mWidth-1);
	} else if (command.compare("A") == 0) {

	} else if (command.compare("B") == 0) {

	} else if (command.compare("select") == 0) {

	} else if (command.compare("start") == 0) {

	} 
}




















