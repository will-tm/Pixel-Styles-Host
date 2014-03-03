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
splitter::splitter(const string &pSrc, const string &pDelim)
{
	reset(pSrc, pDelim);
}

/*
 * destructor
 *
 */
void splitter::reset(const string &pSrc, const string &pDelim)
{
	vector<string> tokens;
	string::size_type start = 0;
	string::size_type end;
	for (;;)
	{
		end = pSrc.find(pDelim, start);
		
		if (pSrc.substr(start, end - start).size() > 0)
			tokens.push_back(pSrc.substr(start, end - start));
		
		if (end == string::npos)
			break;
		
		start = end + pDelim.size();
	}
	mTokens.swap(tokens);
}

/*
 * public functions
 *
 */
string &splitter::operator[](size_type i)
{
	return mTokens.at(i);
}

size_t splitter::size() const
{
	return mTokens.size();
}
