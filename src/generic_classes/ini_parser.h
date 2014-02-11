/*
 * ini_parser.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#ifndef __INI_PARSER_H__
#define __INI_PARSER_H__

using namespace std;

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "strings-helper.h"

/*
 * public class
 *
 */
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

#endif
