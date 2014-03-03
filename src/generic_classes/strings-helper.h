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
template<typename T> std::string to_string(const T& pVar)
{
	ostringstream os;
	os.precision(2);
	os << pVar;
	return os.str();
}

template<typename T> T from_string(const string& pStr)
{
	T value;
	istringstream(pStr) >> value;
	return value;
}

inline void replace_all(string& pStr, const string& pFrom, const string& pTo)
{
	if (pFrom.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = pStr.find(pFrom, start_pos)) != string::npos)
	{
		pStr.replace(start_pos, pFrom.length(), pTo);
		start_pos += pTo.length();
	}
}
