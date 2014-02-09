/********************************************************************************************/
/* Copyright (c) 2013 RGB Styles															*/
/********************************************************************************************/

/********************************************************************************************/
/* This file is the confidential trade secret information and/or proprietary information	*/
/* of RGB Styles, Inc. Code or other information in this program also may be confidential	*/
/* and/or proprietary to RGB Styles, Inc.													*/
/* All rights reserved.																		*/
/********************************************************************************************/

/********************************************************************************************/
/* Name : ini_parser.h																		*/
/* Date : Jul 24 2013																		*/
/* Author : William Markezana																*/
/********************************************************************************************/

#ifndef __INI_PARSER_H__
#define __INI_PARSER_H__

using namespace std;

/********************************************************************************************/
/* INCLUDES																					*/
/********************************************************************************************/
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "strings-helper.h"

/********************************************************************************************/
/* TYPEDEFS																					*/
/********************************************************************************************/


/********************************************************************************************/
/* CLASSES																					*/
/********************************************************************************************/
class ini_parser
{
private:
	string path;
	boost::property_tree::ptree pt;
	void create_file_if_not_exists();
public:
	ini_parser(string _path);
	~ini_parser();

	void save();

	template <typename T> T get(const std::string& section, const std::string& key, const T& default_value) { return pt.get<T>(section+"."+key, default_value); }
	template <typename T> void set(const std::string& section, const std::string& key, const T& value) { pt.put<T>(section+"."+key, value); /*save();*/ }
};

/********************************************************************************************/
/* END OF FILE																				*/
/********************************************************************************************/
#endif
