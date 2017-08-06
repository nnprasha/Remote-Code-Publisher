/////////////////////////////////////////////////////////////////////////////
// Display.cpp - Support Displaying dependencies, typetable and SCs.       //
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
#include "Display.h"
#include "../Analyzer/Executive.h"
#include <iomanip>

//function to display the type table
void Display::showTypeTable(TypeTable& t)
{
	std::cout << "\n\n===============================================================================================================\n";
	std::cout << "\t\t\t\t\t\t TYPE TABLE";
	std::cout << "\n===============================================================================================================\n";

	for (auto it = t.typeMap.begin(); it != t.typeMap.end(); ++it)
	{
		std::vector<ElementType> elements = t.typeMap[it->first];
		for (ElementType elem : elements)
		{
			std::cout << "\n";
			std::cout << "\n  Type: " << elem.type;
			std::cout << "\n  Name of the type: " << it->first;
			std::cout << "\n  File Name (with absolute path): " << elem.path;
			std::cout << "\n  Namespace to which it belongs: " << elem.namespace_;

			std::cout << "\n----------------------------------------------------------------------------------------------------\n";
		}
	}
}

//function to display the dependencies
void Display::showDependencyAnalysis(DepAnal& d)
{
	std::cout << "\n\n ===================================================================================\n";
	std::cout << "\t\t DEPENDENCY TABLE ";
	std::cout << "\n ===================================================================================\n\n";

	for (auto it = d.dependencyMap.begin(); it != d.dependencyMap.end(); ++it)
	{
		std::cout << " FileName: " << it->first<<"\n";
		std::cout << "   Dependencies: ";
		if (d.dependencyMap[it->first].size() == 0) {
			std::cout << "No External Dependencies!!";		
		}
		else
			for (Package elem : d.dependencyMap[it->first])
			{
				std::cout << elem<<"\n\t\t ";
			}
		std::cout << "\n\n----------------------------------------------------------------------------------------------------\n\n";
	}
}

#ifdef TEST_DISPLAY
int main(int argc, char* argv)
{
	CodeAnalysis::CodeAnalysisExecutive exec;
	exec.InitialAnalysis(argc, &argv, exec);
	ScopeStack<ElementType> s;
	TypeTable t;
	exec.ASTdfs(Repository::getInstance()->getGlobalScope(), s, t);
	DepAnal d(t);
	exec.getFilesAndAnalyse(d);
	StrongComponents<std::string, std::string> sc(d.getDepMap());
	sc.SCAnalysis();
	Display display;
	display.showTypeTable(t);
	display.showDependencyAnalysis(d);
	display.showStrongComponents(sc);
}
#endif