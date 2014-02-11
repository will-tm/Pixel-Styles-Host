/*
 * splitter.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#include "splitter.h"

/*
 * constructor
 *
 */
splitter::splitter(const string &src, const string &delim)
{
    reset(src, delim);
}

/*
 * destructor
 *
 */
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

/*
 * public functions
 *
 */
string &splitter::operator[] (size_type i)
{
    return _tokens.at(i);
}

size_t splitter::size() const
{
    return _tokens.size();
}
