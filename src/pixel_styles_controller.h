/*
 * pixel_styles_controller.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

#include "config.h"
#include "settings_list.h"
#include "tcp_server.h"
#include "splitter.h"
#include "led_strip.h"
#include "timer.h"
#include "ini_parser.h"
#include "modes_controller.h"
#include "bonjour.h"
#include "udp_socket.h"

using namespace std;

/*
 * public class
 *
 */
class pixel_styles_controller
{
private:
	bonjour mBonjour;
	vector <rgb_color> mStaticColors;
	mutex mColorsMutex;
	mutex mSettingsMutex;
	settings_list mSettings;
	tcp_server *mTcpServer;
	udp_socket *mUdpSocket;
	led_strip *mStrip;

	modes_controller *mModesController;

	ini_parser *mIniFile;
	timer *mAliveTimer;
	timer *mPaintTimer;
	timer *mPreviewTimer;

	int mFrames;
	vector <size_t> mSegments;

	void handle_tcp_request(string request, string &answer);
	void alive();
	void paint();
	void preview();
public:
	pixel_styles_controller();
	~pixel_styles_controller();

	void run();
};
