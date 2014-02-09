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
/* Name : main.cpp																			*/
/* Date : Sep 03 2012																		*/
/* Author : William Markezana																*/
/********************************************************************************************/

/********************************************************************************************/
/* INCLUDES																					*/
/********************************************************************************************/
#include "pixel_styles_controller.h"

#include <muduo/base/Logging.h>

using namespace std;

/********************************************************************************************/
/* PRIVATE VARIABLES																		*/
/********************************************************************************************/
static pixel_styles_controller *pixelStyles;

/********************************************************************************************/
/* PRIVATE FUNCTIONS																		*/
/********************************************************************************************/
static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << " <option(s)> SOURCES"
              << "Options:\n"
              << "\t-h,--help\t\tShow this help message\n"
              << "\t-k,--killall\tKill all instances of Pixel styles"
              << "\t-q,--quiet\tSilent output"
              << std::endl;
}

static void write_pid_file()
{
	pid_t pid = getpid();
	FILE *fp = fopen("/var/run/pixel_styles.pid", "w");
	if (fp)
	{
		fprintf(fp, "%d\n", pid);
		fclose(fp);
	}
}

static void log_output(const char* msg, int len)
{
	FILE *fp = fopen("/var/log/pixel_styles.log", "a");
	if (fp)
	{
		fprintf(fp, "%s", msg);
		fclose(fp);
	}
	printf("%s",msg);
}

/********************************************************************************************/
/* PROGRAM ENTRY POINT																		*/
/********************************************************************************************/
int main(int argc, char* argv[])
{
	write_pid_file();

	FILE *fp = fopen("/var/log/pixel_styles.log", "w");
	fclose(fp);
	muduo::Logger::setOutput(log_output);

	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];
		if ((arg == "-h") || (arg == "--help"))
		{
			show_usage(argv[0]);
			return 0;
		}
		else if ((arg == "-k") || (arg == "--killall"))
		{
			system("killall -wq pixel_styles");
			return 0;
		}
		else if ((arg == "-q") || (arg == "--quiet"))
		{
			muduo::Logger::setLogLevel(muduo::Logger::FATAL);
		}
		else
		{
			show_usage(argv[0]);
			return 1;
		}
	}

	pixelStyles = new pixel_styles_controller();
	pixelStyles->run();

	while(true) { sleep(1000); };
	return 0;
}

/********************************************************************************************/
/* END OF FILE																				*/
/********************************************************************************************/
