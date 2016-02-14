/*
 * pixel_styles_controller.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "pixel_styles_controller.h"

/*
 * constructor
 *
 */
pixel_styles_controller::pixel_styles_controller()
{
	srand((unsigned) time(0));
	
	mFrames = 0;
	mTcpServer = NULL;
	mUdpSocket = NULL;

	LOG_INFO << "Pixel Styles v" << VERSION << " started";
	
	mIniFile = new ini_parser((string) CONFIGURATION_DIRECTORY + "config.cfg");

	int width = mIniFile->get<size_t>("PIXEL_STYLES", "Width", 19);
	int height = mIniFile->get<size_t>("PIXEL_STYLES", "Height", 12);
	string device = mIniFile->get<string>("PIXEL_STYLES", "SpiDevice", "/dev/spidev0.0");
	strip_bytes_order bytesOrder = mIniFile->get<string>("PIXEL_STYLES", "RgbBytesOrder", "RGB").compare("RGB") ? bgrBytesOrder : rgbBytesOrder;

	mStrip = new led_strip(device.c_str(), bytesOrder, width, height);
	delete mIniFile;
	
	LOG_INFO << "Initialized with size " << width << "x" << height << " on SPI device " << device << " with " << (bytesOrder == rgbBytesOrder ? "RGB" : "BGR") << " bytes order";
	
	mIniFile = new ini_parser((string) SETTINGS_DIRECTORY + "application.cfg");
	
	mAliveTimer = new timer(1000000, bind(&pixel_styles_controller::alive, this));
	mPaintTimer = new timer(10000, bind(&pixel_styles_controller::paint, this));
	mPreviewTimer = new timer(50000, bind(&pixel_styles_controller::preview, this));
	mModesController = new modes_controller(width, height);
	
	mStaticColors.resize(mIniFile->get<size_t>("COLORS", "Count", 1));
	for (size_t i = 0; i < mStaticColors.size(); i++)
	{
		mStaticColors[i] = int_to_rgb(mIniFile->get<uint32_t>("COLORS", "Color" + to_string(i), 0x00FF0000));
	}
	
	mModesController->lock();

	mModesController->set_active_mode_name(mIniFile->get<string>("MODE", "ActiveMode", "Touch"));

	mIniFile->set<string>("MODE", "ActiveMode", mModesController->active_mode_name());
	mModesController->initialize(mStaticColors);

	bool active = mIniFile->get<bool>("MODE", "Active", true);
	mModesController->set_active(active);
	mStrip->setActive(active);

	mModesController->unlock();
	
	LOG_INFO << "pixel_styles_controller initialized";
}

/*
 * destructor
 *
 */
pixel_styles_controller::~pixel_styles_controller()
{
	delete mAliveTimer;
	delete mPaintTimer;
	delete mIniFile;
	delete mModesController;
}

/*
 * public functions
 *
 */

void pixel_styles_controller::handle_tcp_request(string request, string &answer)
{
	splitter split(request, "_");
	
	if (split.size() >= 2 && !split[0].compare("Touch"))
	{
		mColorsMutex.lock();
		size_t touchCount = from_string<int>(split[1]);
		if (split.size() == touchCount + 3 && touchCount >= 1)
		{
			mStaticColors.resize(touchCount);
			mIniFile->set<size_t>("COLORS", "Count", touchCount);
			
			for (size_t i = 0; i < touchCount; i++)
			{
				int intColor = from_string<int>(split[2 + i]);
				rgb_color *colorPtr = (rgb_color*) &intColor;
				rgb_color color = (rgb_color) *colorPtr;
				
				mStaticColors[i] = color;
				mIniFile->set<int>("COLORS", "Color" + to_string(i), intColor);
			}
		}
		mColorsMutex.unlock();
		
		mModesController->lock();
		mModesController->set_active_mode_name("Touch");
		mModesController->active_mode()->touch(mStaticColors, (touch_type) from_string<int>(split[split.size() - 1]));
		mModesController->unlock();
	}
	
	if (split.size() == 4 && !split[0].compare("SetModeSettingValue"))
	{
		mModesController->lock();
		string modeName = split[1];
		mode_interface *mode = mModesController->operator [](modeName);
		if (mode != NULL)
		{
			setting *aSetting = mode->mSettings[split[2]];
			if (aSetting != NULL)
			{
				aSetting->set_value(split[3]);
				answer = mModesController->json_success();
			}
			else
			{
				answer = mModesController->json_error();
			}
		}
		else
		{
			answer = mModesController->json_error();
		}
		mModesController->unlock();
	}
	
	if (split.size() == 2 && !split[0].compare("SetModeName"))
	{
		mModesController->lock();
		mModesController->set_active_mode_name(split[1]);
		mIniFile->set<string>("MODE", "ActiveMode", split[1]);
		answer = mModesController->json_success();
		mModesController->unlock();
	}
	
	if (split.size() == 2 && !split[0].compare("SetActive"))
	{
		bool active = from_string<bool>(split[1]);

		mModesController->lock();
		mModesController->set_active(active);
		mStrip->setActive(active);
		mIniFile->set<int>("MODE", "Active", (int)active);
		mModesController->unlock();
	}

	if (split.size() == 1 && !split[0].compare("GetJSON"))
	{
		mModesController->lock();
		answer = mModesController->to_json();
		mModesController->unlock();
	}
	if (split.size() == 1 && !split[0].compare("GetCurrentModeBitmapJSON"))
	{
		mModesController->lock();
		answer = mModesController->active_mode_bitmap_to_json();
		mModesController->unlock();
	}
}

void pixel_styles_controller::run()
{
	mUdpSocket = new udp_socket(UDP_BROADCAST_PORT);
	
	mModesController->active_mode()->initialize(mStaticColors);
	
	mAliveTimer->run();
	mPaintTimer->run();
	mPreviewTimer->run();
	
	muduo::net::InetAddress listenAddr(TCP_CONNECTION_PORT);
	int maxConnections = 5;
	
	LOG_DEBUG << "maxConnections = " << maxConnections;
	mTcpServer = new tcp_server(get_global_event_loop(), listenAddr, maxConnections);
	mModesController->set_tcp_server(mTcpServer);
	mTcpServer->register_read_callback(bind(&pixel_styles_controller::handle_tcp_request, this, _1, _2));
	mTcpServer->run();
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
	if (connectedAddresses->size())
	{
		mModesController->lock();
		string bitmap = mModesController->active_mode_bitmap_to_json();
		mModesController->unlock();
		
		for (size_t i = 0; i < connectedAddresses->size(); i++)
		{
			mUdpSocket->sendMessageToHost(bitmap, connectedAddresses->operator [](i), 56616);
		}
	}
}
