#ifndef DEPANAL_H
#define DEPANAL_H

//////////////////////////////////////////////////////////////////////////////
// DepAnal.h - Find dependant files for a given file 						//
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Nikhil Prashar, 2017                                        //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2015                             //
// Platform:    HP Spectre, Core i7, Windows 10 Home                       //
// Application: Project 2, CSE687 - Object Oriented Design                 //
// Author:      Nikhil Prashar, Syracuse University                        //
//              (914) 733 8184, nnprasha@syr.edu	                       //
////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* -------------------
* This Package contains a DepAnal Class that enables dependency retrieval
* for a given file. It stores the dependencies locally in unordered map 
* as well as in the NoSqlDB created in the first project. It references the Type Table
* to check for dependencies and if a match is found, it adds the dependency to the Database. 
* 
* Public Interface: 
* -----------------
* NoSqlDB<std::string> dependencyAnalsis(std::string& file) 
*     -Function to retrieve the dependencies for the file name given as an argument.
*
* void AddToNoSqlDB(Key key)
*	  - function called to save the dependencies to the NoSqlDB.*
* 
* std::unordered_map<Package, std::set<Package>> getDepMap()
*	  - Function to retrieve the local unordered map of dependencies. 
* 
* DepAnal()
*	  - DepAnal Constructor that provides an initialization seq to 
*		to instantiate Repo,ScopeStack,AST,Toker and Typetable objects. 
*
* Required Files:
* ---------------
* Parser.h
* ActionsAndRules.h
* TypeAnalyzer.h
* FileSystem.h
* noSqlDB.h
*
* Build Process:
* --------------
* devenv CodeAnalyzer.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 7th March 2017
* - first release
*
*/

#include "../Parser/Parser.h"
#include "../Parser/ActionsAndRules.h"
#include "../TypeAnalyzer/TypeAnalyzer.h"
#include "../FileSystem/FileSystem.h"
#include "../Project1/NoSqlDB/NoSqlDB.h"
#include <string>
#include <set>

using namespace CodeAnalysis;


class DepAnal {
	friend class Display;
	
public:
	DepAnal(TypeTable& _typeTable):
		localRepo(Repository::getInstance()),
		ASTref_(Repository::getInstance()->AST()),
		scopeStack_(Repository::getInstance()->scopeStack()),
		toker_(*(Repository::getInstance()->Toker())),
		tt(_typeTable) {};

	NoSqlDB<std::string> dependencyAnalsis(std::string& file);
	void AddToNoSqlDB(Key key);
	std::unordered_map<Package, std::set<Package>> getDepMap();

private:
	Repository* localRepo;
	AbstrSynTree& ASTref_;
	ScopeStack<ASTNode*> scopeStack_;
	Scanner::Toker& toker_;
	TypeTable& tt;
	std::unordered_map<Package,std::set<Package>> dependencyMap;
	NoSqlDB<std::string> db;
};


#endif // !DEPANAL_H