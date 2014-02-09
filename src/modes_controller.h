/********************************************************************************************/
/* Copyright (c) 2013 RGB Styles															*/
/********************************************************************************************/

/********************************************************************************************/
/* This file is the confidential trade secret information and/or proprietary information	*/
/* of RGB Styles, Inc. Code or other information in this program also may be confidential	*/
/* and/or proprietary to RGB Styles, Inc.													*/
/* All rights reserved.																		*/
/********************************************************************************************/

/********************************************************************************************/
/* Name : modes_controller.h																*/
/* Date : Jul 24 2012																		*/
/* Author : William Markezana																*/
/********************************************************************************************/

#ifndef __MODES_CONTROLLER_H__
#define __MODES_CONTROLLER_H__

using namespace std;

/********************************************************************************************/
/* INCLUDES																					*/
/********************************************************************************************/
#include <linux/types.h>
#include <json_spirit/json_spirit.h>

#include "modes_list.h"

#include "mode_off.h"
#include "mode_touch.h"
#include "mode_plasma.h"

#include "bass.h"
#include "bass_fx.h"
#include "tcp_server.h"
#include "udp_socket.h"

/********************************************************************************************/
/* CLASSES																					*/
/********************************************************************************************/
class modes_controller
{
private:
	size_t mWidth;
	size_t mHeight;
	bool mAudioAvailable;
	modes_list mModesList;
	HRECORD mRecordChannel;
	__u32 mFftData[1024];
	float mAudioLevel;
	float mAudioLevelRatio;
	mode_interface *mActiveMode;
	mutex mModeMutex;
	ini_parser *mIniFile;
	tcp_server *mTcpServer;

	void audio_tasks();
	string string_compress(string const& s);
public:

	modes_controller(size_t pWidth, size_t pHeight);
	~modes_controller();

	void beat_detected();
	void paint();
	bitmap *active_mode_bitmap();
	string active_mode_bitmap_to_json();
	mode_interface *active_mode() { return mActiveMode; }
	void set_active_mode_name(const string pName);
	size_t size() { return mModesList.size(); }
	bool audio_available() { return mAudioAvailable; }
	string to_json();
	string bitmap_to_json(bitmap *pBitmap);
	mode_interface *operator[] (const string name) { return mModesList[name]; }
	void lock() { mModeMutex.lock(); }
	void unlock() { mModeMutex.unlock(); }
	bool try_lock() {  return mModeMutex.try_lock(); }
	void set_tcp_server(tcp_server *pTcpServer) { mTcpServer = pTcpServer; }
	string json_success();
	string json_error();
	void initialize(vector <rgb_color> pStaticColors);
};

/********************************************************************************************/
/* END OF FILE																				*/
/********************************************************************************************/
#endif
