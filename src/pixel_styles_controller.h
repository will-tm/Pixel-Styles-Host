/********************************************************************************************/
/* Copyright (c) 2012 RGB Styles															*/
/********************************************************************************************/

/********************************************************************************************/
/* This file is the confidential trade secret information and/or proprietary information	*/
/* of RGB Styles, Inc. Code or other information in this program also may be confidential	*/
/* and/or proprietary to RGB Styles, Inc.													*/
/* All rights reserved.																		*/
/********************************************************************************************/

/********************************************************************************************/
/* Name : pixel_styles_controller.h															*/
/* Date : Jul 06 2012																		*/
/* Author : William Markezana																*/
/********************************************************************************************/

#ifndef __PIXEL_STYLES_CONTROLLER_H__
#define __PIXEL_STYLES_CONTROLLER_H__

using namespace std;

/********************************************************************************************/
/* INCLUDES																					*/
/********************************************************************************************/
#include "settings_list.h"
#include "tcp_server.h"
#include "splitter.h"
#include "led_strip.h"
#include "timer.h"
#include "ini_parser.h"
#include "modes_controller.h"
#include "bonjour.h"

using namespace std;

/********************************************************************************************/
/* DEFINES																					*/
/********************************************************************************************/

/********************************************************************************************/
/* TYPEDEFS																					*/
/********************************************************************************************/

/********************************************************************************************/
/* CLASSES																					*/
/********************************************************************************************/
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
public:
	pixel_styles_controller();
	~pixel_styles_controller();

	void HandleTcpRequest(string __request, string &__answer, int __clientId);
	void run();
	void alive();
	void paint();
	void preview();
};

/********************************************************************************************/
/* END OF FILE																				*/
/********************************************************************************************/
#endif
