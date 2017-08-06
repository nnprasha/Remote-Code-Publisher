/////////////////////////////////////////////////////////////////////////////
// DepAnal.cpp - Support Dependency analysis for a file                    //
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

#include "DepAnal.h"
#include "../Analyzer/Executive.h"

NoSqlDB<std::string> DepAnal::dependencyAnalsis(std::string& file)
{
	std::unordered_map<Name, std::vector<ElementType>> map = tt.getMap();

	std::ifstream in(file);
	if (in.good())
	{
		toker_.returnComments(false);
		toker_.attach(&in);
		do
		{
			std::string tok = toker_.getTok();
			if (tok != "\n") {
				if (map.find(tok) != map.end())
				{
					std::vector<ElementType> elements = map[tok];
					
					for (ElementType elem : elements)
					{
						if (elem.package != FileSystem::Path::getName(file)) {
							dependencyMap[file].insert(elem.path);
							if (file.substr(file.length() - 3, 3) == "cpp")
							{
								dependencyMap[elem.path].insert(file);
								AddToNoSqlDB(elem.path);
							}
						}
					}										
				}
			}
		} while (in.good());
		AddToNoSqlDB(file); //Adding dependency to nosqldb
	}
	else {
		std::cout << "\n  can't open " << file << "\n\n";
	}
		return db;
}

void DepAnal::AddToNoSqlDB(Key key)
{
	Element<std::string> elem;
	elem.key = key;
	elem.name = key;
	elem.category = "Library";
	elem.desc = "File to check for Dependency.";
	elem.data = "dummy";
	elem.timeDate = time(0);
	for (std::string child : dependencyMap[key])
	{
		elem.children.push_back(child);
	}
	db.SaveToDB(key, elem);
}

std::unordered_map<Package, std::set<Package>> DepAnal::getDepMap() 
{ 
	return dependencyMap;
}

#ifdef TEST_DEPANAL

int main(int argc,char* argv)
{
	CodeAnalysis::CodeAnalysisExecutive exec;
	exec.InitialAnalysis(argc, &argv, exec);
	ScopeStack<ElementType> s;
	TypeTable t;
	exec.ASTdfs(Repository::getInstance()->getGlobalScope(), s, t);
	DepAnal d(t);
	for(auto file : exec.cppHeaderFiles())
		d.dependencyAnalsis(file);
	for (auto file : exec.cppHeaderFiles())
		d.dependencyAnalsis(file);
	for (auto file : exec.csharpFiles())
		d.dependencyAnalsis(file);

	Display display;
	display.showDependencyAnalysis(d);

}

#endif