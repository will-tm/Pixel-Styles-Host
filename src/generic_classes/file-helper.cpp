/*
 * file-helper.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "file-helper.h"

/*
 * public functions
 *
 */
int get_directory_files_list (string dir, vector<string> &files)
{
	DIR *dp;
	struct dirent *dirp;
	if((dp = opendir(dir.c_str())) == NULL)
	{
		cout << "Error(" << errno << ") opening " << dir << endl;
		return errno;
	}
	files = vector<string>();
	while ((dirp = readdir(dp)) != NULL)
	{
		files.push_back(string(dirp->d_name));
	}
	closedir(dp);
	return 0;
}
