/*
 * splitter.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#ifndef __SPLITTER_H__
#define __SPLITTER_H__

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
	vector<string> _tokens;
public:
  	splitter(const string &src, const string &delim);
  	string& operator[] ( size_type i );
  	size_t size() const;
  	void reset ( const string &src, const string &delim);
};

#endif
