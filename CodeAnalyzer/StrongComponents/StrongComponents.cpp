
/////////////////////////////////////////////////////////////////////////////
// StrongComponent.cpp - Support/Test Stub for strong component analysis   //
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
#include "../Analyzer/Executive.h"

#ifdef TEST_STRONGCOMPONENTS

int main(int argc,char* argv)
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
	display.showStrongComponents(sc);
}

#endif