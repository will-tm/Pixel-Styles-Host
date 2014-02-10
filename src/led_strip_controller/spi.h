/*
 * spi.h - Threaded SPI driver
 *
 * Copyright (C) 2013 William Markezana <william.markezana@me.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __SPI_H__
#define __SPI_H__

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <mutex>

class spi
{
private:
	bool mRunning;
	bool mActiveTransfert;
	pthread_t mThreadId;
	struct spi_ioc_transfer mTransfert;
	int mHandle;
	uint8_t mMode;
	std::mutex mMutex;
public:
	spi(const char *pDevice);
   	~spi();

   	void write_buffer(uint8_t *pBuffer, int pLength);
	void run();
	void thread_run();
	bool activeTransfert();
	void waitForTransfertToComplete();
};

#endif
