/*
 * spi.c - Threaded SPI driver
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

#include "spi.h"

spi::spi(const char *pDevice)
{
	mMode = 0;
	mRunning = false;
	mActiveTransfert = false;
	mThreadId = 0;
	mTransfert.rx_buf = 0;
	mTransfert.delay_usecs = 0;
	mTransfert.speed_hz = 2000000;
	mTransfert.bits_per_word = 8;
	mTransfert.cs_change = 0;
	mTransfert.pad = 0;
	mTransfert.len = 0;

	mHandle = open(pDevice, O_RDWR);
	if (mHandle < 0)
	{
		printf("can't open device %s",pDevice);
		exit(1);
	}

	ioctl(mHandle, SPI_IOC_WR_MODE, &mMode);
	ioctl(mHandle, SPI_IOC_RD_MODE, &mMode);
	ioctl(mHandle, SPI_IOC_WR_BITS_PER_WORD, &mTransfert.bits_per_word);
	ioctl(mHandle, SPI_IOC_RD_BITS_PER_WORD, &mTransfert.bits_per_word);
	ioctl(mHandle, SPI_IOC_WR_MAX_SPEED_HZ, &mTransfert.speed_hz);
	ioctl(mHandle, SPI_IOC_RD_MAX_SPEED_HZ, &mTransfert.speed_hz);

	run();
}

spi::~spi()
{
	mRunning = false;
	close(mHandle);
}

static void *thread_run_callback(void *pOwner)
{
	((spi *)pOwner)->thread_run();
	return NULL;
}

void spi::thread_run()
{
	while(mRunning)
	{
		if(mTransfert.len)
		{
			mMutex.lock();
			int bytesWritten = ioctl(mHandle, SPI_IOC_MESSAGE(1), &mTransfert);
			if(bytesWritten < 0)
			{
				printf("write spi error");
				exit(1);
			}
			if(bytesWritten != (int)mTransfert.len)
			{
				printf("Wrote %d bytes instead of %d", bytesWritten, mTransfert.len);
				exit(1);
			}
			mTransfert.len = 0;
			mActiveTransfert = false;
			mMutex.unlock();
		}
		usleep(1000);
	}
	pthread_exit(NULL);
}

void spi::run()
{
	mRunning = true;
	(void)pthread_create(&mThreadId, NULL, thread_run_callback, this);
}

void spi::write_buffer(uint8_t *pBuffer, int pLength)
{
	if(mMutex.try_lock())
	{
		mTransfert.tx_buf = (unsigned long)pBuffer;
		mTransfert.len = pLength;
		mActiveTransfert = true;

		mMutex.unlock();
	}
}

bool spi::activeTransfert()
{
	return mActiveTransfert;
}

void spi::waitForTransfertToComplete()
{
	while(mActiveTransfert);
}
