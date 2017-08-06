#ifndef STRONGCOMPONENTS_H
#define STRONGCOMPONENTS_H
///////////////////////////////////////////////////////////////////////////////
// StrongComponents.h - Constructs Strong Components  						 //
//																			//
// ver 1.0																   //
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
* StrongComponents class constructs a graph by using the Graph class by taking
* every file present in the dependency map as a vertex in a graph and assigns 
* Edges between them which shows the dependencies. Then, using the graph constructed,
* the strong components are found and retrieved. 
*
* Public Interface:
* -----------------
* void SCAnalysis()
*	- The Test Executive calls this function to begin the Strong component Analysis.
*
* void constructGraph()
*	- This function uses the graph class to construct the graph based on the dependency map.
*
* void SCCDfs()
*	- uses dfs to traverse the graph and apply tarjans algorithm.
*
* void SCCHelper(GraphLib::Vertex<V,E> u,int disc[], int low[],
*		std::stack<GraphLib::Vertex<V,E>>* st,bool stackmember[])
*	- Applies tarjans algorithm to retrieve SCs.
*
* std::vector<std::vector<GraphLib::Vertex<V, E>>> getComponents()
*	- returns vector of strong components. 
* 
* void ToXmlSC(StrongComponents<std::string, std::string>& sc,std::string filePath)
*	-Stores Strong Components to XML File
*
* StrongComponents()
*	- Constructor that copies dependencies to the local dependency map from the database. 
*
* Required Files:
* ---------------
* Graph.h
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
#define NOMINMAX


#pragma once
#include "../Graph/Graph/Graph.h"
#include <stack>
#include <set>
#include <unordered_map>
#include <iostream>
#include <vector>


using Package = std::string;


template<typename V,typename E>
class StrongComponents
{
	friend class Display;
public:
	StrongComponents(std::unordered_map<Package, std::set<Package>> dep) :
		dependencies(dep) {};
	void SCAnalysis();
	void constructGraph();
	void SCCDfs();
	void SCCHelper(GraphLib::Vertex<V,E> u,int disc[], int low[],
		std::stack<GraphLib::Vertex<V,E>>* st,bool stackmember[]);
	std::vector<std::vector<GraphLib::Vertex<V, E>>> getComponents();
	void ToXmlSC(std::string& filePath);

private:
	std::unordered_map<Package, std::set<Package>> dependencies;
	GraphLib::Graph<V, E> graph;
	std::vector<std::vector<GraphLib::Vertex<V,E>>> strongComponents;
};

template<typename V,typename E>
void StrongComponents<V, E>::ToXmlSC(std::string& filePath)
{
	std::string xml = "";
	try
	{
		XmlDocument doc;
		SPtr pRoot = makeTaggedElement("StrongComponents");
		doc.docElement() = pRoot;

		for (std::vector<std::vector<GraphLib::Vertex<V, E>>>::iterator it = strongComponents.begin(); it != strongComponents.end(); ++it)
		{
			SPtr pElement = makeTaggedElement("Component"); // <Element>...</Element>
			pRoot->addChild(pElement);

			for (GraphLib::Vertex<V, E> sc : *it)
			{
				SPtr cTag = makeTaggedElement("name");
				pElement->addChild(cTag);
				std::string x = sc.value();
				SPtr cTagText = makeTextElement(x);
				cTag->addChild(cTagText);
			}
		}
		xml = doc.toString();

		std::FILE* myfile;
		fopen_s(&myfile, filePath.c_str(), "w+");
		fprintf(myfile, xml.c_str());
		fclose(myfile);
	}
	catch (std::exception& e)
	{
		std::cout << "Exception e:" << e.what();
	}
}

template<typename V,typename E>
void StrongComponents<V,E>::SCAnalysis()
{
	//Graph construction
	constructGraph();

	//Strong Component Analysis
	SCCDfs();
}

template<typename V, typename E>
void StrongComponents<V,E>::constructGraph()
{
	//Vertex Creation
	for (auto it = dependencies.begin(); it != dependencies.end(); ++it)
	{
		//Create Vertex for each key found in dependency map
		GraphLib::Vertex<V, E> v(it->first);
		graph.addVertex(v);
	}

	//Edge Creation
	for (auto it = dependencies.begin(); it != dependencies.end(); ++it)
	{
		GraphLib::Vertex<V, E> parentV = graph.findVertexByName(it->first);
		for (Package childV : dependencies[it->first])
		{
			std::string edgeVal = parentV.value() + "->" + childV;
			graph.addEdge(edgeVal, parentV, graph.findVertexByName(childV));
		}
	}

}

template<typename V, typename E>
void StrongComponents<V,E>::SCCDfs()
{
	int *disc = new int[graph.size()];
	int *low = new int[graph.size()];
	bool *stackMember = new bool[graph.size()];
	std::stack<GraphLib::Vertex<V, E>>* stack = new std::stack<GraphLib::Vertex<V, E>>();

	// Initialize disc and low, and stackMember arrays
	for (int i = 0; i < graph.size(); i++)
	{
		disc[i] = -1;
		low[i] = -1;
		stackMember[i] = false;
	}

	int i = 0;
	for (GraphLib::Vertex<V, E> vertex : graph.getAdjList())
	{
		if (disc[i] == -1)
			SCCHelper(vertex, disc, low, stack, stackMember);
		i++;
	}
}

template<typename V, typename E>
void StrongComponents<V,E>::SCCHelper(GraphLib::Vertex<V, E> u, int disc[], int low[],
	std::stack<GraphLib::Vertex<V, E>>* st, bool stackMember[])
{
	static int time = 0;

	disc[u.id()] = low[u.id()] = ++time;
	st->push(u);
	stackMember[u.id()] = true;
	for (std::pair<int, E> edge : u.getEdges())
	{
		int v = edge.first;
		if (disc[v] == -1)
		{
			SCCHelper(graph.findVertexById(v), disc, low, st, stackMember);
			low[u.id()] = min(low[u.id()], low[v]);			
		}
		else if (stackMember[v] == true) {
			low[u.id()] = min(low[u.id()], disc[v]);
		}
 	}
	int w = 0;
	if (low[u.id()] == disc[u.id()])
	{
		std::vector<GraphLib::Vertex<V, E>> component;
		while (st->top().id() != u.id())
		{
			w = st->top().id();
			component.push_back(graph.findVertexById(w));
			stackMember[w] = false;
			st->pop();
		}
		w = st->top().id();
		component.push_back(graph.findVertexById(w));
		stackMember[w] = false;
		st->pop();
		strongComponents.push_back(component);
	}
}

template<typename V, typename E>
std::vector<std::vector<GraphLib::Vertex<V, E>>> StrongComponents<V,E>::getComponents()
{ 
	return strongComponents;
}
#endif