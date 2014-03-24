/*
 * spi.cpp - Threaded SPI driver
 *
 * Copyright (C) 2013 William Markezana <william.markezana@me.com>
 *
 */

#include "spi.h"

/*
 * constructor
 *
 */
spi::spi(const char *pDevice)
{
	mMode = 0;
	mRunning = false;
	mActiveTransfert = false;
	mInitializationSuccessful = false;
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
		cerr << "can't open device " << pDevice << endl;
		return;
	}

	ioctl(mHandle, SPI_IOC_WR_MODE, &mMode);
	ioctl(mHandle, SPI_IOC_RD_MODE, &mMode);
	ioctl(mHandle, SPI_IOC_WR_BITS_PER_WORD, &mTransfert.bits_per_word);
	ioctl(mHandle, SPI_IOC_RD_BITS_PER_WORD, &mTransfert.bits_per_word);
	ioctl(mHandle, SPI_IOC_WR_MAX_SPEED_HZ, &mTransfert.speed_hz);
	ioctl(mHandle, SPI_IOC_RD_MAX_SPEED_HZ, &mTransfert.speed_hz);
	
	run();
	mInitializationSuccessful = true;
}

/*
 * destructor
 *
 */
spi::~spi()
{
	mRunning = false;
	close(mHandle);
}

/*
 * private functions
 *
 */
static void *thread_run_callback(void *pOwner)
{
	((spi *) pOwner)->thread_run();
	return NULL;
}

/*
 * public functions
 *
 */
void spi::thread_run()
{
	while (mRunning)
	{
		if (mTransfert.len)
		{
			mMutex.lock();
			int bytesWritten = ioctl(mHandle, SPI_IOC_MESSAGE(1), &mTransfert);
			if (bytesWritten < 0)
			{
				cerr << "write spi error" << endl;
			}
			if (bytesWritten != (int) mTransfert.len)
			{
				cerr << "write spi error" << endl;
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
	(void) pthread_create(&mThreadId, NULL, thread_run_callback, this);
}

void spi::write_buffer(uint8_t *pBuffer, int pLength)
{
	if (mInitializationSuccessful && mMutex.try_lock())
	{
		mTransfert.tx_buf = (unsigned long) pBuffer;
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
	while (mActiveTransfert);
}
