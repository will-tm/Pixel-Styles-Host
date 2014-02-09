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
/* Name : splitter.cpp																		*/
/* Date : Nov 04 2012																		*/
/* Author : William Markezana																*/
/********************************************************************************************/

/********************************************************************************************/
/* INCLUDES																					*/
/********************************************************************************************/
#include "splitter.h"

/********************************************************************************************/
/* PUBLIC FUNCTIONS																			*/
/********************************************************************************************/
splitter::splitter(const string &src, const string &delim)
{
    reset(src, delim);
}

void splitter::reset(const string &src, const string &delim)
{
	vector<string> tokens;
	string::size_type start = 0;
	string::size_type end;
	for (;;)
	{
		end = src.find (delim, start);

		if(src.substr(start, end - start).size() > 0)
			tokens.push_back(src.substr(start, end - start));

		if (end == string::npos)
			break;

		start = end + delim.size();
	}
	_tokens.swap(tokens);
}

string &splitter::operator[] (size_type i)
{
    return _tokens.at(i);
}

size_t splitter::size() const
{
    return _tokens.size();
}

/********************************************************************************************/
/* END OF FILE																				*/
/********************************************************************************************/
