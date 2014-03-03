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
int get_directory_files_list (string pDir, vector<string> &pFiles)
{
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(pDir.c_str())) == NULL)
	{
		cout << "Error(" << errno << ") opening " << pDir << endl;
		return errno;
	}
	pFiles = vector<string>();
	while ((dirp = readdir(dp)) != NULL)
	{
		pFiles.push_back(string(dirp->d_name));
	}
	closedir(dp);
	return 0;
}
