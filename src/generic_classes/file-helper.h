/*
 * file-helper.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

/*
 * public functions
 *
 */
int get_directory_files_list (string pDir, vector<string> &pFiles);
