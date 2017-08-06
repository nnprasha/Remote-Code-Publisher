/////////////////////////////////////////////////////////////////////////////
// TypeAnalyzer.cpp - Support Type Table construction                      //
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
/////////////////////////////////////////////////////////////////////////////

#include "TypeAnalyzer.h"
#include <iomanip>

//Adds a new type to the type table
void TypeTable::add(Name name, ElementType ElementType)
{
	for (auto it = typeMap.begin(); it != typeMap.end(); ++it)
	{
		if (it->first == name)
		{
			typeMap[name].push_back(ElementType);
			return;
		}			
	}
	typeMap[name].push_back(ElementType);
}

//Returns value of a type match
std::vector<ElementType> TypeTable::getValue(Key key)
{
	if (typeMap.find(key) != typeMap.end())
		return typeMap[key];	

	return std::vector<ElementType>();
}
//Dfs over the AST tree to fill the type map
void TypeTable::ASTdfs(CodeAnalysis::ASTNode* pItem, CodeAnalysis::ScopeStack<ElementType>& s)
{
	if (pItem->type_ == "namespace" || pItem->type_ == "class" || pItem->type_ == "struct" || pItem->type_ == "enum" ||
		pItem->type_ == "function")
	{
		static std::string path;
		static size_t indentLevel = 0;
		std::ostringstream out;
		ElementType elem;
		elem.name = pItem->name_;

		if (pItem->type_ == "function")
			elem.type = "Global function";
		else
			elem.type = pItem->type_;

		if (pItem->name_ == "Global Namespace")
		{
			elem.package = "NULL";
			elem.path = "NULL";
			elem.namespace_ = "Global Namespace";
		}
		else
		{
			elem.package = pItem->package_;
			elem.path = pItem->path_;
		}

		if (s.size() > 0)
			elem.namespace_ = s.top().name;

		if (elem.type == "namespace")
			s.push(elem);
		add(elem.name, elem);

		auto iter = pItem->children_.begin();
		++indentLevel;
		while (iter != pItem->children_.end())
		{
			CodeAnalysis::ASTNode* x = *iter;
			if (!(x->type_ == "function" && pItem->type_ != "namespace"))
				ASTdfs(*iter, s);
			++iter;
		}
		if (pItem->type_ == "namespace")
			s.pop();
		--indentLevel;
	}
}

//Returns type map
std::unordered_map<Name, std::vector<ElementType>> TypeTable::getMap() 
{ 
	return typeMap;
}

#ifdef TEST_TYPEANALYZER

int main(int argc,char* argv)
{
	CodeAnalysis::CodeAnalysisExecutive exec;	
	exec.InitialAnalysis(argc, &argv, exec);
	ScopeStack<ElementType> s;
	TypeTable t;
	exec.ASTdfs(Repository::getInstance()->getGlobalScope(), s, t);
	Display d;
	d.showTypeTable(t);
}
#endif // TEST_TYPEANALYZER
