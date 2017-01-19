/*
 * modes_controller.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

using namespace std;

#include <linux/types.h>
#include <json_spirit.h>

#include "file-helper.h"

#include "modes_list.h"

#include "mode_off.h"
#include "mode_touch.h"
#include "mode_fading.h"

#include "bass.h"
#include "bass_fx.h"
#include "tcp_server.h"
#include "udp_server.h"

/*
 * public class
 *
 */
class modes_controller
{
private:
	size_t mWidth;
	size_t mHeight;
	bool mAudioAvailable;
	bool mBypassBASS;
	modes_list mModesList;
	HRECORD mRecordChannel;
	uint32_t mIntFftData[1024];
	float mFftData[1024];
	float mAudioLevel;
	float mAudioLevelRatio;
	mode_interface *mActiveMode;
	mutex mModeMutex;
	vector<size_t> mSegments;
	ini_parser *mIniFile;
	tcp_server *mTcpServer;
	udp_server *mUdpServer;
	uint32_t mLastUdpFrameTick;
	vector<size_t> mSpectrum;
	vector<size_t> mScope;
	vector<size_t> mPows;
	float mAutoAmpGain;
	float mAutoAmpAverage;
	float mAutoAmpRawAverage;
	bool mActive;
	int mBrightness;

	void add_dynamic_modes(size_t pWidth, size_t pHeight, bool pAudioAvailable, vector<size_t> pSegments);
	void audio_tasks();
	void handle_receive(uint8_t *pData, size_t pLength);
public:

	modes_controller(size_t pWidth, size_t pHeight);
	~modes_controller();

	void beat_detected();
	void paint();
	bitmap *active_mode_bitmap();
	string active_mode_bitmap_to_json();
	string active_mode_name();
	void set_active_mode_name(const string pName);
	string to_json();
	string json_success();
	string json_error();
	void initialize(vector<rgb_color> pStaticColors);
	void process_fft_buffer_1024(float *pFftdata);
	string bitmap_to_json(bitmap *pBitmap, mode_interface *pMode);

	mode_interface *active_mode()
	{
		return mActiveMode;
	}
	size_t size()
	{
		return mModesList.size();
	}
	bool audio_available()
	{
		return mAudioAvailable;
	}
	mode_interface *operator[](const string pName)
	{
		return mModesList[pName];
	}
	void lock()
	{
		mModeMutex.lock();
	}
	void unlock()
	{
		mModeMutex.unlock();
	}
	bool try_lock()
	{
		return mModeMutex.try_lock();
	}
	void set_tcp_server(tcp_server *pTcpServer)
	{
		mTcpServer = pTcpServer;
	}
	void set_active(bool active)
	{
		mActive = active;
	}
	void set_brightness(int brightness)
	{
		mBrightness = brightness;
	}
};
