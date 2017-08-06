#ifndef DISPLAY_H
#define DISPLAY_H
//////////////////////////////////////////////////////////////////////////////
// Display.h - Displayes Dependency table, Type table  						//
//				and strong components.										//
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
* The Test Executive uses this module to display the Dependency table,
* Type table and strong components for a given set of files. The Display Class 
* is a friend class for TypeAnalyzer, DepAnal and StrongComponent class. 
*
* Public Interface:
* -----------------
* void showTypeTable(TypeTable& t)
*	- Displays the typetable that was constructed for the set of files. 
*
* void showDependencyAnalysis(DepAnal& d)
*	- Displays the Depedencies from the database in the form of a dependency table.
*
* void showStrongComponents(StrongComponents<V,E>& sc)
*	- displays the strong components for a given set of files (As vertices of a graph). 
*
* Required Files:
* ---------------
* TypeAnalyzer.h
* StrongComponents.h
* Graph.h
* DepAnal.h
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
#include "../TypeAnalyzer/TypeAnalyzer.h"
#include "../StrongComponents/StrongComponents.h"
#include "../Graph/Graph/Graph.h"
#include "../DepAnal/DepAnal.h"

class Display {
public:
	void showTypeTable(TypeTable& t);
	void showDependencyAnalysis(DepAnal& d);

	template<typename V, typename E>
	void showStrongComponents(StrongComponents<V,E>& sc);
};

//function to display strong components. 
template<typename V, typename E>
void Display::showStrongComponents(StrongComponents<V,E>& sc)
{
	std::cout << "\n\n ===================================================================================\n";
	std::cout << "\t\t STRONG COMPONENTS ";
	std::cout << "\n ===================================================================================\n\n";
	int i = 1;
	for (std::vector<GraphLib::Vertex<V, E>> component : sc.strongComponents)
	{
		std::cout << "Strong Component " << i<<":  { \n\t\t\t";
		for (auto it = component.begin(); it != component.end(); ++it)
		{
			GraphLib::Vertex<V, E> v = *it;
			std::cout << v.value() << "\n\t\t\t";
		}
		std::cout << "\b\b\b}\n\n\n";
		i++;
	}
	
}

#endif // !DISPLAY_H
