#ifndef MSGCLIENT_H
#define MSGCLIENT_H
#pragma once
//////////////////////////////////////////////////////////////////////////////
// MsgClient.h - Client for the socket connection							//
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
* Module Operations:
* -------------------
* This Package contains the Client class for the socket connection with the Server. 
* It contains methods for the send and receieve functionalities. 
*
* Public Interface:
* -----------------
* void UploadFiles(std::string& files,std::string& category);
*		--> Upload files to the server repository.
*
* std::string PublishFiles(std::string& msg);
*		--> Publish files present at the Server side
*
*  MsgClient() - Constructor that makes a connection with the server
*
*	std::string DownloadFiles(std::string& msg) - Downloads files from the server repository
*	std::string& GetFiles(std::string& msg) - Gets list of source files from the server repositoty
*	void GetSelectedHtmFiles(std::string& msg) - Gets selected HTML Files from the server repository
*	std::string LazyDownload(std::string& msg) - Gets lazy download files from the server repository
*	std::string DeleteSelectedFiles(std::string& msg) - delete selected files from the server repository
*	std::string IISPublish(std::string msg) - Published HTM files to IIS
*	void openStartUpPage(std::string webPageRepoPath, std::string cat) - opens index page
*	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep) - constructs http message
*	void sendMessage(HttpMessage& msg, Socket& socket) - sends message to server
*	bool sendFile(const std::string& fqname, Socket& socket,std::string& category) - sends files block by block to the server
*
* Required Files:
* ---------------
* HttpMessage.h
* Sockets.h
* FileSystem.h
* Logger.h
* Utilities.h
* 
* Build Process:
* --------------
* devenv CodeAnalyzer.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 2nd April 2017
* - first release
*
*/
#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"

class MsgClient
{
public:
	using EndPoint = std::string;
	void UploadFiles(std::string& files,std::string& category);
	std::string PublishFiles(std::string& msg);
	MsgClient();
	std::string DownloadFiles(std::string& msg);
	std::string& GetFiles(std::string& msg);
	void GetSelectedHtmFiles(std::string& msg);
	std::string LazyDownload(std::string& msg);
	std::string DeleteSelectedFiles(std::string& msg);
	std::string IISPublish(std::string msg);
	void openStartUpPage(std::string webPageRepoPath, std::string cat);
	std::string NoParentFile(std::string& msg);

private:
	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);
	void sendMessage(HttpMessage& msg, Socket& socket);
	bool sendFile(const std::string& fqname, Socket& socket,std::string& category);
	SocketSystem ss;
	SocketConnecter si;
};


#endif
