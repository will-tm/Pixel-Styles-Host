/********************************************************************************************/
/* Copyright (c) 2012 RGB Styles                              								*/
/********************************************************************************************/

/********************************************************************************************/
/* This file is the confidential trade secret information and/or proprietary information  	*/
/* of RGB Styles, Inc. Code or other information in this program also may be confidential 	*/
/* and/or proprietary to RGB Styles, Inc.                         							*/
/* All rights reserved.                                   									*/
/********************************************************************************************/

/********************************************************************************************/
/* Name : strings-helper.h                                 									*/
/* Date : Aug 30 2012                                   									*/
/* Author : William Markezana                               								*/
/********************************************************************************************/

#pragma once

/********************************************************************************************/
/* INCLUDES                                         										*/
/********************************************************************************************/
#include <iostream>
#include <cstdio>
#include <sstream>
#include <string>

using namespace std;

/********************************************************************************************/
/* TEMPLATES                                         										*/
/********************************************************************************************/

template <typename T> std::string to_string(const T& t)
{
	std::ostringstream os;
	os.precision(2);
	os << t;
	return os.str();
}

template <typename T> T from_string(const std::string& s)
{
	T value;
	std::istringstream(s) >> value;
	return value;
}

inline void replace_all(string& str, const string& from, const string& to)
{
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos)
	{
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

/********************************************************************************************/
/* END OF FILE                                        										*/
/********************************************************************************************/
