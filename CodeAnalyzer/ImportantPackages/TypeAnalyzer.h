#ifndef TYPEANALYZER_H
#define TYPEANALYZER_H

#pragma once

//////////////////////////////////////////////////////////////////////////////
// TypeAnalyzer.h - Type Table container									//
// ver 1.0																	//
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
* This module declares 2 classes, Element class and TypeTable class. 
* The type table class stores the types retrieved from a set of files
* in a TypeMap of Element types. 
*
* Public Interface:
* -----------------
* Type type- Type of the Element. 
* Name name- Type name of the element.
* Package package- container file for the specified type. 
* Path path- File path for the package. 
* Namespace namespace- Container namespace of the specified type. 
*
* void add(Name name, ElementType element)
*	-Adds a type to the type table (type map).
*
* std::vector<ElementType> getValue(Key key)
*	-Gets a value for a specific type(key). 
*
* std::unordered_map<Name, std::vector<ElementType>> getMap()
*	-Returns the type table map. *
* 
* void ASTdfs(CodeAnalysis::ASTNode* pItem, CodeAnalysis::ScopeStack<ElementType>& s)
*	-DFS on AST tree to fill the type table
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
#include<string>
#include<iostream>
#include<unordered_map>
#include "../AbstractSyntaxTree/AbstrSynTree.h"
#include "../ScopeStack/ScopeStack.h"

using Type = std::string;
using Name = std::string;
using Package = std::string;
using Path = std::string;
using Namespace = std::string;
using Key = Name;

//class that specifies properties of different type elements
class ElementType {
public:
	Type type;
	Name name;
	Package package;
	Path path;
	Namespace namespace_;
};

//Class that stores the typtTable map<Name,vector<Element>>
class TypeTable {
	friend class Display;
public:
	void add(Name name, ElementType element);
	std::unordered_map<Name, std::vector<ElementType>> getMap();
	std::vector<ElementType> getValue(Key key);
	void ASTdfs(CodeAnalysis::ASTNode* pItem, CodeAnalysis::ScopeStack<ElementType>& s);

private:
	std::unordered_map<Name, std::vector<ElementType>> typeMap;
};


#endif // !TYPEANALYZER_H
