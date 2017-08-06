//////////////////////////////////////////////////////////////////////////////
// NoDqlDB.cpp - No Sql Database											//
// ver 1.0																	//
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
#include <ctime>
#include <time.h>
#include <locale.h>
#include <sstream>
#include <thread>
#include <Windows.h>






#ifdef NO_TEST_NOSQLDB

int main()
{
	using StringData = std::string;
	using IntegerData = int;
	using Key = NoSqlDB<StringData>::Key;
	using AllKeys = NoSqlDB<StringData>::AllKeys;

	std::cout << "-------------------------------------------------------------";
	std::cout << "\n\tWELCOME TO NO-SQL DATABASE\n";
	std::cout << "-------------------------------------------------------------";
	try {
		TestExecutive<StringData> stringTX;	//Creating Test Executive Object
		NoSqlDB<StringData> stringDb;	//Creting NoSqlDB Object(String Databse)
	//	stringTX.XmlInput(stringDb, "../XmlInputs/NoSqlDB-ProjectStructure.xml", true);	//XMl Input and creation of String Database

		std::cout << "\n\n Swithcing On Timed Saving of Database to XML (Requirement number 6, Line number 22-34 in NoSqlDB.cpp)";
		stringDb.ScheduleSave(true);	//Scheduled saving being switched on

		std::cout << "\n------------------------------------------------------------------\n";
		std::cout << "\tREQUIREMENT NO 3 and 4- ADDITION OF NEW ELEMENTS (and adding child relationships)";
		std::cout << "\n------------------------------------------------------------------\n";
		stringTX.AddKey(stringDb);
		if (!stringTX.QueryManager(stringDb))
			std::cout << "\n\nQueries could not be successfully excecuted due to the above mention exception!!";
		std::cout << "\n------------------------------------------------------------------\n";
		std::cout << "\tREQUIREMENT NO 3 and 4- DELETION OF ELEMENTS FROM DATABASE (and deletion of child relationships)";
		std::cout << "\n------------------------------------------------------------------\n";
		stringTX.DeleteKey(stringDb);
		std::cout << "\n------------------------------------------------------------------\n";
		std::cout << "\tREQUIREMENT NO 4- EDITING OF DATABASE";
		std::cout << "\n------------------------------------------------------------------\n";
		stringTX.DatabaseEditing(stringDb);

		Sleep(100);
		std::cout << "\n\n Swithcing Off Timed Saving of Database to XML (Requirement number 6, Line number 67-68 in NoSqlDB.cpp)";
		stringDb.ScheduleSave(false);
		//<--------------- Requirement 2 for INTEGER DATABASE -------------------->

		NoSqlDB<IntegerData> intDb;		//NoSql IntDB	
		//TestExecutive<IntegerData> intTX;
	//	intTX.XmlInput(intDb, "../XmlInputs/NoSqlDb-Integer.xml", false);
	}
	catch (std::exception& e)
	{		std::cout << "****Exception has been thrown: " << e.what();	}
	catch (...)
	{		std::cout<< "******Some unknown error has been thrown. Please rebuild and run!!";	}
	std::cout << "\n\n";	
	std::cin.get();
}

#endif // TEST_NOSQLDB
