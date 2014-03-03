/*
 * ini_parser.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

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

	template<typename T> T get(const std::string& pSection, const std::string& pKey, const T& pDefaultValue)
	{
		return pt.get<T>(pSection + "." + pKey, pDefaultValue);
	}
	template<typename T> void set(const std::string& pSection, const std::string& pKey, const T& pValue)
	{
		pt.put<T>(pSection + "." + pKey, pValue);
		save();
	}
};

