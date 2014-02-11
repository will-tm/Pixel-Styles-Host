/*
 * string-helper.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

#include <iostream>
#include <cstdio>
#include <sstream>
#include <string>

using namespace std;

/*
 * public templates
 *
 */
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
