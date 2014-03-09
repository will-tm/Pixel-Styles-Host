/*
 * spi.h - Threaded SPI driver
 *
 * Copyright (C) 2013 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;
/*
 * public class
 *
 */
class spi
{
private:
	bool mInitializationSuccessful;
	bool mRunning;
	bool mActiveTransfert;
	thread *mTransmitThread;
	struct spi_ioc_transfer mTransfert;
	int mHandle;
	uint8_t mMode;
	mutex mMutex;
	condition_variable mConditionVariable;

	void thread_run();
public:
	spi(const char *pDevice);
	~spi();

	void write_buffer(uint8_t *pBuffer, int pLength);
	void run();
	bool activeTransfert();
	void waitForTransfertToComplete();
};
