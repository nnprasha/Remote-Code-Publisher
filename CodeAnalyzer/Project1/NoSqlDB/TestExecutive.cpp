/////////////////////////////////////////////////////////////////////////////
// TestExecutive.cpp - Demonstrates all the project requirements           //
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

#ifdef TEST_TESTEXECUTIVE
using StrData = std::string;

	int main()
	{
		NoSqlDB<StrData> db;
		TestExecutive<StrData> tx;
		
		//Adding new key/value pair to the database
		tx.AddKey(db);

		//Deleting key/value pairs from the database
		tx.DeleteKey(db);

		//Editing the database
		tx.DatabaseEditing(db);

		//Executing queries on the database
		tx.QueryManager(db);
	}
#endif 

