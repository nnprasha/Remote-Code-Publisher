#ifndef QUERY
#define QUERY 
//////////////////////////////////////////////////////////////////////////////
// Cpp11-Query.h - Query Class												//
// ver 1.0																   //
// ----------------------------------------------------------------------- //
// copyright © Nikhil Prashar, 2017                                        //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2015                             //
// Platform:    HP Spectre, Core i7, Windows 10 Home                       //
// Application: Project 1, CSE687 - Object Oriented Design                 //
// Author:      Nikhil Prashar, Syracuse University                        //
//              (914) 733 8184, nnprasha@syr.edu	                       //
////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
*This package contains one Query<Data> Class that defines the
*that executes all the different queries that the user wants 
*execute. Several functions are defined to execute the different
*Queries. 
*
* Public Interface:
*------------------
* 
* Element<Data> GetValueFromKey(NoSqlDB<Data>&,Keys AllKeys,Key specifiedKey)
*	-Query number 1 to return value for specified key
*
* Children GetChildrenFromKey(NoSqlDB<Data>&, Key specifiedKey)
*	-Query number 2 to return children for specified key
*
* Keys GetKeysFromPattern(NoSqlDB<Data>&, std::string specifiedString)
*	-Query number 3 to return key from a particular pattern
*
* Keys GetKeysWithSpecifiedStringInItemName(NoSqlDB<Data>&,std::vector<Key> AllKeys,std::string specifiedString)
*	-Query number 4 tp return keys with specific item name
*
* Keys GetKeysWithSpecifiedStringInCategoryName(NoSqlDB<Data>&, std::vector<Key> AllKeys, std::string specifiedString)
*	-Query number 5 to return keys with specific Category name
*
* Keys GetKeysWithinSpecifiedTimeinterval(NoSqlDB<Data>&, std::vector<Key> AllKeys, std::string begin, std::string end)
*	-Query number 6 to return keys with specific time interval
*	
* Keys GetKeysWithSpecifiedStringInTemplateData(NoSqlDB<Data>&, std::vector<Key> AllKeys)
*	-Query number 7 to check for keys with specific String in data.
*
* Required Files:
* ---------------
* NoSqlDB.h
*
* Build Process:
* --------------
* devenv codeAnalyzer.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 7th Feb 2017
* - first release
*
* ver 1.1 : 7th March 2017
* - Second Release
*/

#include "NoSqlDB.h"
#include <regex>


template <typename Data>
class Query {
public:

	using Key = std::string;
	using Keys = std::vector<Key>;
	using Children = std::vector<Key>;

	Element<Data> GetValueFromKey(NoSqlDB<Data>&,Keys AllKeys,Key specifiedKey);
	Children GetChildrenFromKey(NoSqlDB<Data>&, Key specifiedKey);
	Keys GetKeysFromPattern(NoSqlDB<Data>&, std::string specifiedString);
	Keys GetKeysWithSpecifiedStringInItemName(NoSqlDB<Data>&,std::vector<Key> AllKeys,std::string specifiedString);
	Keys GetKeysWithSpecifiedStringInCategoryName(NoSqlDB<Data>&, std::vector<Key> AllKeys, std::string specifiedString);
	Keys GetKeysWithinSpecifiedTimeinterval(NoSqlDB<Data>&, std::vector<Key> AllKeys, std::string begin, std::string end);
	Keys GetKeysWithSpecifiedStringInTemplateData(NoSqlDB<Data>&, std::vector<Key> AllKeys);
};



//Query No 1 - The value of a specified key.
template <typename Data>
Element<Data> Query<Data>::GetValueFromKey(NoSqlDB<Data>& db, Keys AllKeys, Key specifiedKey)
{
	std::cout << "\n--> Query1- Returning the value of the specified key: " << specifiedKey;
	for (Key key : AllKeys)
	{
		if (key == specifiedKey)
		{
			Element<Data> elem = db.RetrieveValueFromDB(key);
			std::cout << elem.show();
			return elem;
		}			
	}
	std::cout << "\n\t-->Query1 has not returned any output. The specified key does not exist in the database";
	return Element<Data>();
}

//Query No 2 - The children of a specified key.
template <typename Data>
std::vector<std::string> Query<Data>::GetChildrenFromKey(NoSqlDB<Data>& db, Key specifiedKey)
{			
	Element<Data> elem = db.RetrieveValueFromDB(specifiedKey);
	if (elem.key.getValue() == "")
	{
		std::cout << "\n\t-->Query2 failed. Key: " << specifiedKey << " not present in the database";
		return Children();
	}
		
	if (elem.children.size() > 0)
	{
		std::cout << "\n-->Query2- Returning Children of a specified Key: " << specifiedKey;
			for (Key key : elem.children)
				std::cout << "\n\t-->" << key;
	}
	else
		std::cout << "\n--> The specified key does not contain any children";

	return elem.children;	
}

//Query No 3- Set of all keys matching a specified pattern
template <typename Data>
std::vector<std::string> Query<Data>::GetKeysFromPattern(NoSqlDB<Data>& db, std::string specifiedString)
{
	Keys AllKeys = db.RetrieveAllKeys();
	Keys result;
	std::cout << "\n\n--> Query3- Return all keys that contain the specified string : \"" << specifiedString << "\"";
	std::ostringstream out;
	out << "(.*)" << specifiedString << "(.*)";
	std::string pattern = out.str();
	for (Key key : AllKeys)
	{
		if (std::regex_match(key.c_str(), std::regex(pattern), std::regex_constants::match_default))
		{
			std::cout << "\n\t-->Key: " << key;
			result.push_back(key);
		}
	}
	if (result.size() == 0)
		std::cout << "\nNo keys present that contain the specified string: \"" << specifiedString << "\"";

	return result;
}

//Query No 4 - All keys that contain a specified string in their item name
template <typename Data>
std::vector<std::string> Query<Data>::GetKeysWithSpecifiedStringInItemName(NoSqlDB<Data>& db,std::vector<Key> AllKeys,std::string specifiedString)
{
	Keys result;
	std::ostringstream out;
	std::cout << "\n\t-->Query No 4- Return all keys from a given set of keys that contain a specified string: \"" << specifiedString << "\" in their item name. ";
	std::cout << "\n\t\t-->Set of keys that are given(Obtained in Query2): <";
	for (Key key : AllKeys)
		std::cout << key << ",";
	std::cout << ">";
	out << "(.*)" << specifiedString << "(.*)";
	std::string pattern = out.str();
	std::cout << "\n\t\t--> Keys that contain the specifiedString \""<<specifiedString<<"\" in their item name: ";
	for (Key key : AllKeys)
	{
		Element<Data> elem = db.RetrieveValueFromDB(key);
		std::string name = elem.name;
		if (std::regex_match(name.c_str(), std::regex(pattern), std::regex_constants::match_default))
		{
			std::cout << "\n\t\t\t-->";
			result.push_back(key);
			std::cout << key;
		}
	}
	if (result.size() == 0)
		std::cout << " NULL (NO KEYS)";
	return result;
}

//Query No 5
template<typename Data>
std::vector<std::string> Query<Data>::GetKeysWithSpecifiedStringInCategoryName(NoSqlDB<Data>& db, std::vector<Key> AllKeys, std::string specifiedString)
{
	Keys result;
	std::ostringstream out;
	std::cout << "\n\t-->Query No 5- Return all keys from a given set of keys that contain a specified string: \"" << specifiedString << "\" in their category name. ";
	std::cout << "\n\t\t-->Set of keys that are given(Obtained in Query3): <";
	for (Key key : AllKeys)
		std::cout << key << ",";
	std::cout << ">";
	out << specifiedString << "(.*)";
	std::string pattern = out.str();
	std::cout << "\n\t\t--> Keys that start with a specifiedString \""<< specifiedString<<"\" in their category name: ";
	for (Key key : AllKeys)
	{
		Element<Data> elem = db.RetrieveValueFromDB(key);
		std::string name = elem.category;
		if (std::regex_match(name.c_str(), std::regex(pattern), std::regex_constants::match_default))
		{
			std::cout << "\n\t\t\t-->";
			result.push_back(key);
			std::cout << key;
		}
	}
	if (result.size() == 0)
		std::cout << " NULL (NO KEYS)";
	return result;
}

//Query No 6-All keys that contain a specified string in their template data section when that makes sense.
template<typename Data>
std::vector<std::string> Query<Data>::GetKeysWithSpecifiedStringInTemplateData(NoSqlDB<Data>& db, std::vector<Key> AllKeys)
{
	Keys result;
	std::cout << "\n\t-->Query No 6- Return all keys with string data in database.";
	std::cout << "\n\t\t-->Set of given keys(Union of keys obtained in query 2 and 3): <";
	for (Key key : AllKeys)
		std::cout << key << ",";
	std::cout << ">";
	std::cout << "\n\t\t-->Keys that satisfy the Query: ";
		for (Key key : AllKeys)
		{
			Element<Data> elem = db.RetrieveValueFromDB(key);
			if (typeid(elem.data) == typeid(Property<std::string>))
			{
				std::cout << "\n\t\t\t-->";
				result.push_back(key);
				std::cout << key;
			}
		}
	if (result.size() == 0)
		std::cout << " NULL (NO KEYS)";
	return result;
}

//Query No 7
template<typename Data>
std::vector<std::string> Query<Data>::GetKeysWithinSpecifiedTimeinterval(NoSqlDB<Data>& db,std::vector<Key> AllKeys,std::string begin, std::string end)
{
	std::cout << "\n\t-->Query No 7- Return all keys within specified time period.";
	if (strcmp(end.c_str(), "") == 0)
		end = DateTime_ToString(time(0));

	std::cout << "\n\t\t-->Set of given keys(Union of keys obtained in query 4 and 5): <";
	for (Key key : AllKeys)
		std::cout << key << ",";
	std::cout << ">";
	std::cout << "\n\t\t-->Begin time: " << begin;
	std::cout << "\n\t\t-->End time:" << end;

	std::cout << "\n\t\t All Keys within the above specified time periods: ";	
	Keys result;
	for (Key key : AllKeys)
	{
		Element<Data> elem = db.RetrieveValueFromDB(key);
		if (difftime(elem.timeDate, DateTime_FromString(begin.c_str())) < 0)
			continue;
		if (difftime(elem.timeDate, DateTime_FromString(end.c_str())) <= 0)
		{
			std::cout << "\n\t\t\t-->";
			result.push_back(key);
			std::cout << key;
		}
	}

	if (result.size() == 0)
		std::cout << " NULL (NO KEYS)";
	return result;
}

#endif // !QUERY