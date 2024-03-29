/*
 * splitter.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

using namespace std;

#include <string>
#include <vector>
#include <iostream>

/*
 * public types
 *
 */
typedef vector<string>::size_type size_type;

/*
 * public class
 *
 */
class splitter
{
private:
	vector<string> mTokens;
public:
	splitter(const string &pSrc, const string &pDelim);
	string& operator[](size_type pIndex);
	size_t size() const;
	void reset(const string &pSrc, const string &pDelim);
};

