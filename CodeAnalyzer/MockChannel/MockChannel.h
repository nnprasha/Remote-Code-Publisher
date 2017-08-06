#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H
//////////////////////////////////////////////////////////////////////////////
// MockChannel.h -Interface between the client and GUIs.					//
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
// Source:      Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015  //
////////////////////////////////////////////////////////////////////////////
/*
*	This package provides an interface between the GUI and the client using C++ Cli. 

*  Build Process:
* --------------
* devenv CodeAnalyzer.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 2nd April 2017
* - first release
*
*/

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>
using Message = std::string;

struct ISendr
{
  virtual void postMessage(const Message& msg) = 0;
};

struct IRecvr
{
  virtual std::string getMessage() = 0;
};

struct IMockChannel
{
public:
  virtual void start() = 0;
  virtual void stop() = 0;
};

extern "C" {
  struct ObjectFactory
  {
    DLL_DECL ISendr* createSendr();
    DLL_DECL IRecvr* createRecvr();
    DLL_DECL IMockChannel* createMockChannel(ISendr* pISendr, IRecvr* pIRecvr);
  };
}

#endif


