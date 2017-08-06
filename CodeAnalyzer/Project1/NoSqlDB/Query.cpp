//////////////////////////////////////////////////////////////////////////////
// Query.cpp -  Executes Queries over the Database 							//
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
#include "TestExecutive.h"

#ifdef TEST_QUERY


int main()
{
	using StrData = std::string;

	TestExecutive<StrData> tx;
	NoSqlDB<StrData> db;
	tx.XmlInput(db, "../XmlInputs/NoSqlDB-ProjectStructure.xml", true);

	Query<StrData> query;

	Element<StrData> value=query.GetValueFromKey(db, db.RetrieveAllKeys(), "Query");
	Keys children = query.GetChildrenFromKey(db, "Query");
	Keys keys1 = query.GetKeysFromPattern(db, "Xml");
	Keys keys2 = query.GetKeysWithinSpecifiedTimeinterval(db, keys1, "2017-01-01:19:55:48", "");
	Keys keys3 = query.GetKeysWithSpecifiedStringInCategoryName(db, keys2, "Lib");
	Keys keys4 = query.GetKeysWithSpecifiedStringInItemName(db, keys3, "Q");
	Keys keys5 = query.GetKeysWithSpecifiedStringInTemplateData(db, keys4);
	
}


#endif // TEST_QUERY
