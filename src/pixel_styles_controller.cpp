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
/* Name : pixel_styles_controller.cpp														*/
/* Date : Jul 07 2013																		*/
/* Author : William Markezana																*/
/********************************************************************************************/

/********************************************************************************************/
/* INCLUDES																					*/
/********************************************************************************************/
#include "pixel_styles_controller.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

/********************************************************************************************/
/* PRIVATE PROTOTYPES																		*/
/********************************************************************************************/
static void DidReceiveStringCallback(string __request, string &__answer, int __clientId, void *pParent);
static void AliveTimerCallback(void *pParent);
static void PaintTimerCallback(void *pParent);
static void PreviewTimerCallback(void *pParent);

/********************************************************************************************/
/* CONSTRUCTOR																				*/
/********************************************************************************************/
pixel_styles_controller::pixel_styles_controller()
{
	srand((unsigned)time(0));
	// ================================== INITIALIZATIONS ====================================
	mFrames = 0;
	mTcpServer = NULL;
	mUdpSocket = NULL;

	mIniFile = new ini_parser("/etc/pixel_styles/config.cfg");
	int width = mIniFile->get<size_t>("PIXEL_STYLES","With",19);
	int height = mIniFile->get<size_t>("PIXEL_STYLES","Height",12);
	string device = mIniFile->get<string>("PIXEL_STYLES","SpiDevice","/dev/spidev0.0");
	mStrip = new led_strip(device.c_str(), width, height);
	delete mIniFile;

	mIniFile = new ini_parser("/etc/pixel_styles/settings_application.cfg");

	mAliveTimer = new timer(1000000, AliveTimerCallback, this);
	mPaintTimer = new timer(10000, PaintTimerCallback, this);
	mPreviewTimer = new timer(50000, PreviewTimerCallback, this);
	mModesController = new modes_controller(width, height);

	mStaticColors.resize(mIniFile->get<size_t>("COLORS", "Count", 1));
	for(size_t i = 0; i < mStaticColors.size(); i++)
	{
		int intColor = mIniFile->get<int>("COLORS", "Color"+to_string(i), 0x00FF0000);
		rgb_color *colorPtr = (rgb_color*)&intColor;
		rgb_color color = (rgb_color)*colorPtr;
		mStaticColors[i] = color;
	}

	mModesController->lock();
	mModesController->set_active_mode_name(mIniFile->get<string>("MODE", "ActiveMode", "Touch"));
	mModesController->initialize(mStaticColors);
	mModesController->unlock();

	LOG_INFO << "pixel_styles_controller initialized";
}

/********************************************************************************************/
/* DESTRUCTOR																				*/
/********************************************************************************************/
pixel_styles_controller::~pixel_styles_controller()
{
	delete mAliveTimer;
	delete mPaintTimer;
	delete mIniFile;
	delete mModesController;
}
/********************************************************************************************/
/* PRIVATE FUNCTIONS																		*/
/********************************************************************************************/
static void DidReceiveStringCallback(string __request, string &__answer, int __clientId, void *pParent)
{
	((pixel_styles_controller*)pParent)->HandleTcpRequest(__request, __answer, __clientId);
}

static void AliveTimerCallback(void *pParent)
{
	((pixel_styles_controller*)pParent)->alive();
}

static void PaintTimerCallback(void *pParent)
{
	((pixel_styles_controller*)pParent)->paint();
}

static void PreviewTimerCallback(void *pParent)
{
	((pixel_styles_controller*)pParent)->preview();
}

void pixel_styles_controller::HandleTcpRequest(string __request, string &__answer, int __clientId)
{
	splitter split(__request, "_");

	if(split.size() >= 2 && !split[0].compare("Touch"))
	{
		mColorsMutex.lock();
		size_t touchCount = from_string<int>(split[1]);
		if(split.size() == touchCount + 3 && touchCount >= 1)
		{
			mStaticColors.resize(touchCount);
			mIniFile->set<size_t>("COLORS", "Count", touchCount);

			for(size_t i = 0; i < touchCount; i++)
			{
				int intColor =from_string<int>(split[2+i]);
				rgb_color *colorPtr = (rgb_color*)&intColor;
				rgb_color color = (rgb_color)*colorPtr;

				mStaticColors[i] = color;
				mIniFile->set<int>("COLORS", "Color"+to_string(i), intColor);
			}
		}
		mColorsMutex.unlock();

		mModesController->lock();
		mModesController->set_active_mode_name("Touch");
		mModesController->active_mode()->touch(mStaticColors, (touch_type)from_string<int>(split[split.size()-1]));
		mModesController->unlock();
	}

	if(split.size() == 4 && !split[0].compare("SetModeSettingValue"))
	{
		mModesController->lock();
		string modeName = split[1];
		mode_interface *mode = mModesController->operator [](modeName);
		if(mode != NULL)
		{
			setting *aSetting = mode->mSettings[split[2]];
			if(aSetting != NULL)
			{
				aSetting->set_value(split[3]);
				__answer = mModesController->json_success();
			}
			else
			{
				__answer = mModesController->json_error();
			}
		}
		else
		{
			__answer = mModesController->json_error();
		}
		mModesController->unlock();
	}

	if(split.size() == 2 && !split[0].compare("SetModeName"))
	{
		mModesController->lock();
		mModesController->set_active_mode_name(split[1]);
		//mModesController->active_mode()->initialize(mStaticColors);
		mIniFile->set<string>("MODE", "ActiveMode", split[1]);
		__answer = mModesController->json_success();
		mModesController->unlock();
	}

	if(split.size() == 1 && !split[0].compare("GetJSON"))
	{
		mModesController->lock();
		__answer = mModesController->to_json();
		mModesController->unlock();
	}

	if(split.size() == 1 && !split[0].compare("GetCurrentModeBitmapJSON"))
	{
		mModesController->lock();
		__answer = mModesController->active_mode_bitmap_to_json();
		mModesController->unlock();
	}
}

/********************************************************************************************/
/* PUBLIC FUNCTIONS																			*/
/********************************************************************************************/
void pixel_styles_controller::run()
{
	mUdpSocket = new udp_socket();

	muduo::Logger::setLogLevel(muduo::Logger::INFO);

	mModesController->active_mode()->initialize(mStaticColors);

	mAliveTimer->run();
	mPaintTimer->run();
	mPreviewTimer->run();

	muduo::net::EventLoop loop;
	muduo::net::InetAddress listenAddr(56615);
	int maxConnections = 5;

	LOG_DEBUG << "maxConnections = " << maxConnections;
	mTcpServer = new tcp_server(&loop, listenAddr, maxConnections);
	mModesController->set_tcp_server(mTcpServer);
	mTcpServer->registerCallback(&DidReceiveStringCallback, this);
	mTcpServer->run();

	LOG_INFO << "main loop start running...";
	loop.loop();
}

void pixel_styles_controller::alive()
{
	LOG_DEBUG << "Frames per second = " << mFrames;
	mFrames = 0;
}

void pixel_styles_controller::paint()
{
	mModesController->lock();
	mModesController->paint();
	mModesController->unlock();

	mStrip->paint(mModesController->active_mode_bitmap(), false, false);

	mFrames++;
}

void pixel_styles_controller::preview()
{
	std::vector<std::string> *connectedAddresses = mTcpServer->connectedAddresses();
	if(connectedAddresses->size())
	{
		mModesController->lock();
		string bitmap = mModesController->active_mode_bitmap_to_json();
		mModesController->unlock();

		for(size_t i = 0; i < connectedAddresses->size(); i++)
		{
			mUdpSocket->sendMessageToHost(bitmap, connectedAddresses->operator [](i), 56616);
		}
	}
}
/********************************************************************************************/
/* END OF FILE																				*/
/********************************************************************************************/
