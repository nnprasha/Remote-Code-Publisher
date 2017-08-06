#ifndef CODEPUBLISHER_H
#define CODEPUBLISHER_H
//////////////////////////////////////////////////////////////////////////////
// CodePublisher.h - Publish Files present in a repository					//
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
* This Package contains a CodePublisher class. It performs HTML tag creation and 
* code publishing functionality. Also, it uses JavaScript to expland/collapse
* method bodies on mouse click. It uses ShellExecute to open a startup html window. 
*
* Public Interface:
* -----------------
*CodePublisher(std::string,std::string,std::string,std::string);
*	-->Constructor that initialzes paths to web pages. 
*
*void copytoHtmlFiles(const std::vector<std::string>& files, NoSqlDB<std::string>& db);
*	-->This function copies all data from .h and .cpp files and replaces << >> with &lt; &gt;
*
*std::vector<std::string> extractDependencyFromDB(NoSqlDB<std::string>& db, const std::string& file);
*	-->This function extracts dependencies from the database to create Dependency tags in html file.
*
*void buildHtmlHeaderAndcopy(std::string& fileContent, const std::string& file, NoSqlDB<std::string>& db, std::vector<std::string> deps);
*	-->This function builds the Html Header Content and copies it to the html file created. 
*
*std::string buildDependencyDisplayTags(const std::string& file, std::vector<std::string> deps);
*	-->This function builds the dependency tags.

*void hideOnMouseClick(std::string& fileContent);
*	--> This function provides the javascript functionality of expanding/collapsing method bodies. 

*void buildStartUpPage(const std::string& rows);
*	--> This function builds the start up webpage. 

*void openStartUpPage();
*	--> This function uses ShellExecute to open a web page automatically. 
*
*void createHtmlCommonHeaders();
*	--> This function creates the common html tags and stores it to a file!!
*
* Required Files:
* ---------------
* NoSqlDb.h
* ScopeStack.h
* FileSystem.h
*
* Build Process:
* --------------
* devenv CodeAnalyzer.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 4th april 2017
* - first release
*
*/
#pragma once

#include "../Project1/NoSqlDB/NoSqlDB.h"
#include "../ScopeStack/ScopeStack.h"
#include "../FileSystem/FileSystem.h"

class CodePublisher {
public:	
	CodePublisher(std::string,std::string,std::string,std::string,std::vector<std::string>);
	void copytoHtmlFiles(const std::vector<std::string>& files, NoSqlDB<std::string>& db);
	std::vector<std::string> extractDependencyFromDB(NoSqlDB<std::string>& db, const std::string& file);
	void buildHtmlHeaderAndcopy(std::string& fileContent, const std::string& file, NoSqlDB<std::string>& db, std::vector<std::string> deps);
	std::string buildDependencyDisplayTags(const std::string& file, std::vector<std::string> deps);
	void hideOnMouseClick(std::string& fileContent);
	void buildStartUpPage(const std::string& rows,std::vector<std::string> a);
	void openStartUpPage();
	void createHtmlCommonHeaders();

private:
	NoSqlDB<std::string> db;
	std::string scriptingFilesPath;
	std::string webPageRepoPath;
	std::string htmlHeader;
	std::string startPageHeader;
	std::string startUpPageName;
	std::string cTime;
	std::vector<std::string> impPackages;
};

#endif