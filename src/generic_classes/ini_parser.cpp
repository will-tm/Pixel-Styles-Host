/*
 * ini_parser.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "ini_parser.h"

/*
 * constructor
 *
 */
ini_parser::ini_parser(string _path)
{
	path = _path;
	create_file_if_not_exists();
	read_ini(path, pt);
}

/*
 * destructor
 *
 */
ini_parser::~ini_parser()
{
	pt.clear();
}

/*
 * private functions
 *
 */
void ini_parser::create_file_if_not_exists()
{
	FILE *file;
	bool file_exists;
	
	file = fopen(path.c_str(), "r");
	if (file == NULL)
	{
		file_exists = false;
	}
	else
	{
		file_exists = true;
		fclose(file);
	}
	
	if (file_exists)
	{
		file = fopen(path.c_str(), "r+b");
	}
	else
	{
		file = fopen(path.c_str(), "w+b");
	}
	
	if (file != NULL)
	{
		fclose(file);
	}
}

/*
 * public functions
 *
 */
void ini_parser::save()
{
	write_ini(path, pt);
}
