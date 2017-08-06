////////////////////////////////////////////////////////////////////////////
// MsgServer.cpp - Demonstrates simple one-way HTTP style messaging       //
//                 and file transfer                                      //
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
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* -------------------
* This package implements a server that receives HTTP style messages and
* files from multiple concurrent clients and simply displays the messages
* and stores files.
*
* Public Interface:
* -----------------
* void SendHtmFiles(std::string& cat) - Sends html files to the client
* void GetFileNameOnCat(std::string& cat) - sends source files to client
* void sendSelectedHtmFiles(std::string& csvFiles) - send selected html files to client
* void lazyDownload(std::string& csvFile) - sends lazy download files to client
* void DeleteSelectedFiles(std::string& msg) - deletes selected files
* void callPublisher(std::string& category, bool forLazy = false) - publishes files
* void IISPublish(std::string msg) - published files to IIS
* void IISPublishHelper(std::string& path, std::string& cat, std::string& cssPath, Socket& si) - IIS Helper function
* void LazyHelper(bool& lazy, std::string& retStr) - LAzy download helper function
* void deleteSrcFiles(std::vector<std::string>&, std::string& filesDeleted) - delete source code files
* void deleteHtmFiles(std::vector<std::string>&, std::string& filesDeleted) - deletes HTML FIles
* void MainHelper(HttpMessage& msg) - Helper to demonstrate functionalities.
* ClientHandler(BlockingQueue<HttpMessage>& msgQ) - initializes blocking queue to handle client messages
* void operator()(Socket socket) - operator runs on a seperate thread to process client requests
* void ProcessFileMsg(Socket& socket, HttpMessage& msg, std::string& filename) - Processes file messages helper fn
*
* Required Files:
* ---------------
* HttpMessage.h
* Sockets.h
* FileSystem.h
* Logger.h
* Utilities.h
* Cpp11-BlockingQueue.h
* Executive.h
*
* Build Process:
* --------------
* devenv CodeAnalyzer.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 2nd April 2017
* - first release
*/

#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Cpp11-BlockingQueue.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../Analyzer/Executive.h"
#include <string>
#include <iostream>
#include <algorithm>

//using namespace CodeAnalysis;
using namespace Logging;
using namespace Async;
using Show = StaticLogger<1>;
using namespace Utilities;

class ClientHandler
{
public:
  using EndPoint = std::string;
  ClientHandler(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
  void operator()(Socket socket);
  void ProcessFileMsg(Socket& socket, HttpMessage& msg, std::string& filename);

private:
  bool connectionClosed_;
  HttpMessage readMessage(Socket& socket);
  bool readFile(const std::string& filename, size_t fileSize, Socket& socket,std::string& category);
  BlockingQueue<HttpMessage>& msgQ_;
};
//----< this defines processing to frame messages >------------------

class MsgServer
{
public:
	using EndPoint = std::string;
	void SendHtmFiles(std::string& cat);
	void GetFileNameOnCat(std::string& cat);
	void sendSelectedHtmFiles(std::string& csvFiles);
	void lazyDownload(std::string& csvFile);
	void DeleteSelectedFiles(std::string& msg);
	void callPublisher(std::string& category, bool forLazy = false);
	void IISPublish(std::string msg);
	void IISPublishHelper(std::string& path, std::string& cat, std::string& cssPath, Socket& si);
	void LazyHelper(bool& lazy, std::string& retStr);
	//void deleteSrcFiles(std::vector<std::string>&, std::string& filesDeleted);
	void deleteHtmFiles(std::vector<std::string>&, std::string& filesDeleted, std::string& cat);
	void MainHelper(HttpMessage& msg);
	void NoParentFiles(std::string& msg);
	void NoParentFilesHelper(std::string& path, std::string& npf);
private:
	NoSqlDB<std::string> db;
	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);
	void sendMessage(HttpMessage& msg, Socket& socket);
	bool sendFile(const std::string& fqname, Socket& socket,std::string& cat,bool selectedFile = false, bool lazy = false);
};

//Send files to the client
bool MsgServer::sendFile(const std::string& filename, Socket& socket,std::string& cat,bool selectedFile, bool lazy)
{
	// assumes that socket is connected

	std::string fqname = "../RemoteWebPageRepository/" + cat + "/" + filename;
	FileSystem::FileInfo fi(fqname);
	size_t fileSize = fi.size();
	std::string sizeString = Converter<size_t>::toString(fileSize);
	FileSystem::File file(fqname);
	file.open(FileSystem::File::in, FileSystem::File::binary);
	if (!file.isGood())
		return false;

	HttpMessage msg = makeMessage(1, "", "localhost::8081");
	msg.addAttribute(HttpMessage::Attribute("file", filename));
	if (lazy) {
		std::string cTime = DateTime_ToString(time(0));
		std::replace(cTime.begin(), cTime.end(), ':', '-');
		cTime = "LazyDownload-" + cTime;
		msg.addAttribute(HttpMessage::Attribute("Category", cTime));
		msg.addAttribute(HttpMessage::Attribute("LazyDownload","AllFiles"));
	}
	else if(selectedFile)
		msg.addAttribute(HttpMessage::Attribute("Category", "SelectedFilesDownloadFolder"));
	/*if(!selectedFile)
		msg.addAttribute(HttpMessage::Attribute("Category", cat));*/
	else
		msg.addAttribute(HttpMessage::Attribute("Category", cat));
	msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
	sendMessage(msg, socket);
	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];
	while (true)
	{
		FileSystem::Block blk = file.getBlock(BlockSize);
		if (blk.size() == 0)
			break;
		for (size_t i = 0; i < blk.size(); ++i)
			buffer[i] = blk[i];
		socket.send(blk.size(), buffer);
		if (!file.isGood())
			break;
	}
	file.close();
	return true;
}

//Construct a client
HttpMessage MsgServer::makeMessage(size_t n, const std::string& body, const EndPoint& ep)
{
	HttpMessage msg;
	HttpMessage::Attribute attrib;
	EndPoint myEndPoint = "localhost:8080";  // ToDo: make this a member of the sender
											 // given to its constructor.
	switch (n)
	{
	case 1:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("POST", "Message"));
		msg.addAttribute(HttpMessage::Attribute("mode", "oneway"));
		msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
		msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + myEndPoint));

		msg.addBody(body);
		if (body.size() > 0)
		{
			attrib = HttpMessage::attribute("content-length", Converter<size_t>::toString(body.size()));
			msg.addAttribute(attrib);
		}
		break;
	default:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("Error", "unknown message type"));
	}
	return msg;
}

//Send a message to the client
void MsgServer::sendMessage(HttpMessage& msg, Socket& socket)
{
	std::string msgString = msg.toString();
	socket.send(msgString.size(), (Socket::byte*)msgString.c_str());
}

//Helper function for read message
void ClientHandler::ProcessFileMsg(Socket& socket, HttpMessage& msg, std::string& filename)
{
	if (filename != "")
	{
		// construct message body

		msg.removeAttribute("content-length");
		std::string bodyString = "<file>" + filename + "</file>";
		std::string sizeString = Converter<size_t>::toString(bodyString.size());
		msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
		msg.addBody(bodyString);
	}
	else
	{
		// read message body

		size_t numBytes = 0;
		size_t pos = msg.findAttribute("content-length");
		if (pos < msg.attributes().size())
		{
			numBytes = Converter<size_t>::toValue(msg.attributes()[pos].second);
			Socket::byte* buffer = new Socket::byte[numBytes + 1];
			socket.recv(numBytes, buffer);
			buffer[numBytes] = '\0';
			std::string msgBody(buffer);
			msg.addBody(msgBody);
			delete[] buffer;
		}
	}
}

//Read message from client
HttpMessage ClientHandler::readMessage(Socket& socket)
{
  connectionClosed_ = false;
  HttpMessage msg;

  // read message attributes

  while (true)
  {
    std::string attribString = socket.recvString('\n');
    if (attribString.size() > 1)
    {
      HttpMessage::Attribute attrib = HttpMessage::parseAttribute(attribString);
      msg.addAttribute(attrib);
    }
    else
    {
      break;
    }
  }
  // If client is done, connection breaks and recvString returns empty string

  if (msg.attributes().size() == 0)
  {
    connectionClosed_ = true;
    return msg;
  }
  // read body if POST - all messages in this demo are POSTs

  if (msg.attributes()[0].first == "POST")
  {
    // is this a file message?

    std::string filename = msg.findValue("file");
    if (filename != "")
    {
      size_t contentSize;
      std::string sizeString = msg.findValue("content-length");
      if (sizeString != "")
        contentSize = Converter<size_t>::toValue(sizeString);
      else
        return msg;
	  std::string category = msg.attributes()[msg.findAttribute("Category")].second;
      readFile(filename, contentSize, socket, category);
    }
	ProcessFileMsg(socket, msg, filename);
  }
  return msg;
}

//----< read a binary file from socket and save >--------------------
/*
 * This function expects the sender to have already send a file message, 
 * and when this function is running, continuosly send bytes until
 * fileSize bytes have been sent.
 */
bool ClientHandler::readFile(const std::string& filename, size_t fileSize, Socket& socket, std::string& category)
{
	if(!FileSystem::Directory::exists("../RemoteServerRepository/" + category))
		FileSystem::Directory::create("../RemoteServerRepository/" + category);

	std::string fqname = "../RemoteServerRepository/" + category + "/" + FileSystem::Path::getName(filename);
 
	std::cout << "\n\n --> Reading file: " << filename <<" (*** REQUIREMENT 5 and 8)";
	std::cout << "\n\n Adding file to category: " << category;

  FileSystem::File file(fqname);
  file.open(FileSystem::File::out, FileSystem::File::binary);
  if (!file.isGood())
  {
    Show::write("\n\n  can't open file " + fqname);
    return false;
  }

  const size_t BlockSize = 2048;
  Socket::byte buffer[BlockSize];

  size_t bytesToRead;
  while (true)
  {
    if (fileSize > BlockSize)
      bytesToRead = BlockSize;
    else
      bytesToRead = fileSize;

    socket.recv(bytesToRead, buffer);

    FileSystem::Block blk;
    for (size_t i = 0; i < bytesToRead; ++i)
      blk.push_back(buffer[i]);

    file.putBlock(blk);
    if (fileSize < BlockSize)
      break;
    fileSize -= BlockSize;
  }
  file.close();
  return true;
}

//----< receiver functionality is defined by this function >---------

void ClientHandler::operator()(Socket socket)
{
  while (true)
  {
    HttpMessage msg = readMessage(socket);
    if (connectionClosed_ || msg.bodyString() == "quit")
    {
      Show::write("\n\n  clienthandler thread is terminating");
      break;
    }
    msgQ_.enQ(msg);
  }
}

//Calls the code publisher by specifying required arguments
void MsgServer::callPublisher(std::string& category, bool forLazy)
{
	Show::stop();
	std::cout << "\n\n --> Message received with instruction to publish all files in category: " << category <<" (** REQUIREMENT 5, 6, 7)\n\n";
	std::string path="../RemoteServerRepository/" + category;
	std::string retStr = "";
	if (FileSystem::Directory::exists(path))
	{
		char* a[12];
		a[0] = "";
		a[1] = (char*)path.c_str();
		a[2] = "*.h";
		a[3] = "*.cpp";
		a[4] = "/ScriptingPath";
		a[5] = "../ScriptingFiles/";
		a[6] = "/WebPageRepo";
		a[7] = "../RemoteWebPageRepository/";
		a[8] = "/HtmlHeader";
		a[9] = "HtmlHeader.htm";
		a[10] = "/StartPageHeader";
		a[11] = "StartUpPageHeader.htm";	
		CodeAnalysisExecutive exec;
		exec.InitialAnalysis(12, a, exec);
		exec.ExtractFromCommandLine(12, a);
		std::vector<std::string> x;
		//x.push_back("asd");
		db = exec.DemoReq9(x);
		retStr = "Published Successfully in folder (On the server) - ../RemoteWebPageRepository/" + category;
		std::cout << "\n\n --> Published successfull. Check path for files: " << path;
	}
	else {
		std::cout << "\n\n --> Publish Unsuccessful." << category << " does not exists";
		retStr = "Unsucccessful. " + category + " does not exits";
	}
	
	LazyHelper(forLazy, retStr);

}

//Helper for lazy download
void MsgServer::LazyHelper(bool& forLazy, std::string& retStr)
{
	if (!forLazy)
	{
		try {
			SocketSystem ss;
			SocketConnecter si;
			while (!si.connect("localhost", 8081))
			{
				Show::write("\n server waiting to connect");
				::Sleep(100);
			}
			//Send return message
			HttpMessage retMsg = makeMessage(1, "", "localhost::8081");
			retMsg.addAttribute(HttpMessage::Attribute("PublishRetMessage", retStr));
			sendMessage(retMsg, si);
			//Send quit message
			HttpMessage quitmsg = makeMessage(1, "quit", "localhost::8081");
			sendMessage(quitmsg, si);
		}
		catch (std::exception& exc)
		{
			Show::write("\n  Exeception caught: ");
			std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
			Show::write(exMsg);
		}
	}
}

//Send html files back to the client, based on category
void MsgServer::SendHtmFiles(std::string& cat)
{
	try
	{
		SocketSystem ss;
		SocketConnecter si;
		while (!si.connect("localhost", 8081))
		{
			Show::write("\n server waiting to connect");
			::Sleep(100);
		}
		std::vector<std::string> files = FileSystem::Directory::getFiles("../RemoteWebPageRepository/"+cat, "*.htm");
		if (files.size() < 1)
		{
			std::cout << "\n\n --> No files present in category: " << cat;
			HttpMessage noFiles = makeMessage(1, "", "localhost::8081");
			noFiles.addAttribute(HttpMessage::Attribute("NoFilesPresent", cat));
			sendMessage(noFiles, si);
			return;
		}
		for (size_t i = 0; i < files.size(); ++i)
		{
			std::cout <<"\n\n  sending file " << files[i];
			sendFile(files[i], si,cat);
		}
		//Send quit message
		HttpMessage quitmsg = makeMessage(1, "quit", "localhost::8081");
		sendMessage(quitmsg, si);		
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

//Send souce code files to client
void MsgServer::GetFileNameOnCat(std::string& cat)
{
	try
	{
		SocketSystem ss;
		SocketConnecter si;
		while (!si.connect("localhost", 8081))
		{
			Show::write("\n server waiting to connect");
			::Sleep(100);
		}

		std::string path = "../RemoteServerRepository/" + cat;
		std::vector<std::string> files = FileSystem::Directory::getFiles(path, "*.*");
		if (files.size() < 1)
		{
			//Send message to client
			HttpMessage getFileMsg = makeMessage(1, "", "localhost::8081");
			getFileMsg.addAttribute(HttpMessage::Attribute("GetFiles", "No files in specified category"));
			sendMessage(getFileMsg, si);
			si.close();
			std::cout << "No files present in required category";
			return;
		}

		std::string fileMsgBody = "";
		for (std::string file : files)
			fileMsgBody += file + ",";

		std::cout << "\n\n --> Sending back list of files to client : " << fileMsgBody;
		//Send message to client
		HttpMessage getFileMsg = makeMessage(1, "", "localhost::8081");
		getFileMsg.addAttribute(HttpMessage::Attribute("GetFiles", fileMsgBody));
		sendMessage(getFileMsg, si);

		//Send quit message
		HttpMessage quitmsg = makeMessage(1, "quit", "localhost::8081");
		sendMessage(quitmsg, si);
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

//send back selected html files to client
void MsgServer::sendSelectedHtmFiles(std::string& csvFiles)
{
	try {

		SocketSystem ss;
		SocketConnecter si;
		while (!si.connect("localhost", 8081))
		{
			Show::write("\n server waiting to connect");
			::Sleep(100);
		}

		std::vector<std::string> filesToSearch = Utilities::StringHelper::split(csvFiles);
		filesToSearch.erase(filesToSearch.begin());
		std::string category = filesToSearch.at(0);
		filesToSearch.erase(filesToSearch.begin());
		std::vector<std::string> filesToSend;
		//std::vector<std::string> dirs = FileSystem::Directory::getDirectories("../RemoteWebPageRepository", "Cat*");
		for (std::string f : filesToSearch)
		{		
			std::vector<std::string> cFile = FileSystem::Directory::getFiles("../RemoteWebPageRepository/" + category + "/", f + ".htm");
			
			if (cFile.size() > 0)
			{
				if (cFile.at(0) == f+".htm")
				{
					std::cout << "\n\n --> sending htm file : " << f + ".htm";
					sendFile(f+".htm", si, category,true,false);
				}
			}				
		}

		std::cout << "\n\n server sending quit message to client***";
		//Send quit message
		HttpMessage quitmsg = makeMessage(1, "quit", "localhost::8081");
		sendMessage(quitmsg, si);
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

//Lazy download from client
void MsgServer::lazyDownload(std::string& csvFile)
{
	try {
		SocketSystem ss;
		SocketConnecter si;
		while (!si.connect("localhost", 8081))
		{
			Show::write("\n server waiting to connect");
			::Sleep(100);
		}
		std::cout << "\n\n --> Server received message for lazy download of files: " << csvFile;
		std::string file = Utilities::StringHelper::split(csvFile).at(2);
		std::string path = "../RemoteServerRepository/" + Utilities::StringHelper::split(csvFile).at(1) + "/" +
			Utilities::StringHelper::split(csvFile).at(2);
		path = FileSystem::Path::getFullFileSpec(path); //fully qualified path
		std::set<std::string> lazyFiles;
		ScopeStack<std::string> s;
		s.push(path);
		while (s.size() > 0)
		{
			std::string f = s.pop();
			lazyFiles.insert(f);
			Element<std::string> elem = db.RetrieveValueFromDB(f);
			for (std::string c : elem.children)
			{
				const bool is_in = lazyFiles.find(c) != lazyFiles.end();
				if(!is_in)
					s.push(c);
			}		
		}
		for (std::string f : lazyFiles)
		{
			std::string cat = Utilities::StringHelper::split(csvFile).at(1);
			sendFile(FileSystem::Path::getName(f) + ".htm", si, cat, false,true);
			std::cout << " \n \n sending lazy file: " << FileSystem::Path::getName(f) + ".htm";
		}
		std::cout << "\n\n server sending quit message to client***";
		//Send quit message
		HttpMessage quitmsg = makeMessage(1, "quit", "localhost::8081");
		sendMessage(quitmsg, si);
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

//Delete selected files from server repo
void MsgServer::DeleteSelectedFiles(std::string& msg)
{
	try {
			SocketSystem ss;
			SocketConnecter si;
			while (!si.connect("localhost", 8081))
			{
				Show::write("\n server waiting to connect");
				::Sleep(100);
			}
			std::cout << "\n\n ----> Server received message to delete files : "<<msg;
			std::vector<std::string> filesToDelete = Utilities::StringHelper::split(msg);
			std::string filesDeleted = "";
			filesToDelete.erase(filesToDelete.begin());
			std::string category = filesToDelete.at(0);
			filesToDelete.erase(filesToDelete.begin());

			//Deleting source code files from server repository
			//deleteSrcFiles(filesToDelete, filesDeleted);

			//Deleting htm files from server repository
			deleteHtmFiles(filesToDelete, filesDeleted, category);

			//Send back message of deleted files
			HttpMessage msg = makeMessage(1, "", "localhost::8081");
			if (filesDeleted == "")
				filesDeleted = "Html file of the selected src file has already been deleted before.";

			msg.addAttribute(HttpMessage::Attribute("DeletedFiles",filesDeleted));
			sendMessage(msg, si);


			std::cout << "\n\n server sending quit message to client***";
			//Send quit message
			HttpMessage quitmsg = makeMessage(1, "quit", "localhost::8081");
			sendMessage(quitmsg, si);
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

//Delete source code files from server Repository
//void MsgServer::deleteSrcFiles(std::vector<std::string>& filesToDelete, std::string& filesDeleted)
//{
//	std::vector<std::string> dirs = FileSystem::Directory::getDirectories("../RemoteServerRepository/","Cat*");
//	for (std::string d : dirs)
//	{
//		std::vector<std::string> filesInDirectory = FileSystem::Directory::getFiles("../RemoteServerRepository/" + d);
//
//		for (std::string fToDelete : filesToDelete)
//		{
//			for (std::string fInD : filesInDirectory)
//			{
//				if (fInD == fToDelete)
//				{
//					std::string x = FileSystem::Path::getFullFileSpec("../RemoteServerRepository/" + d + "/" + fToDelete);
//					bool b=FileSystem::File::remove(x);
//					filesDeleted += fToDelete + ",";
//				}
//			}
//		}
//
//		if (FileSystem::Directory::getFiles("../RemoteServerRepository/" + d).size() == 0)
//			FileSystem::Directory::remove("../RemoteServerRepository/" + d);
//	}
//}

//Delete Htm Files
void MsgServer::deleteHtmFiles(std::vector<std::string>& filesToDelete, std::string& filesDeleted, std::string& cat)
{
	//std::vector<std::string> htmdirs = FileSystem::Directory::getDirectories("../RemoteWebPageRepository/","Cat*");
	
	std::vector<std::string> htmfilesInDirectory = FileSystem::Directory::getFiles("../RemoteWebPageRepository/" + cat + "/");

	for (std::string fToDelete : filesToDelete)
	{
		for (std::string fInD : htmfilesInDirectory)
		{
			if (fInD == fToDelete + ".htm")
			{
				FileSystem::File::remove(FileSystem::Path::getFullFileSpec("../RemoteWebPageRepository/" + cat + "/" + fToDelete + ".htm"));
				filesDeleted += fToDelete + ".htm" + ",";
				std::cout << "\n\n --> Deleting file: " << fToDelete + ".htm";
			}
		}
	}
/*
	if (FileSystem::Directory::getFiles("../RemoteServerRepository/" + cat).size() == 0)
		FileSystem::Directory::remove("../RemoteServerRepository/" + cat);*/
	
}

//Publish to IIS Server
void MsgServer::IISPublish(std::string msg)
{
	try {
			SocketSystem ss;
			SocketConnecter si;
			while (!si.connect("localhost", 8081))
			{
				Show::write("\n server waiting to connect");
				::Sleep(100);
			}
			std::vector<std::string> message = Utilities::StringHelper::split(msg);
			message.erase(message.begin());
			std::string cat = message.at(0);
			std::string path = message.at(1);
			std::string cssPath = path.substr(0, path.find_last_of("\\/"));
			if (!FileSystem::Directory::exists(path))
			{
				std::cout << "\n\n --> IIS FOLDER PATH DOES NOT EXISTS : " << path;
				HttpMessage msg = makeMessage(1, "", "localhost::8081");
				msg.addAttribute(HttpMessage::Attribute("IISRet", "IIS FOLDER PATH DOES NOT EXISTS"));
				sendMessage(msg, si);
				//Send quit message
				HttpMessage quitmsg = makeMessage(1, "quit", "localhost::8081");
				sendMessage(quitmsg, si);
				return;
			}
			if (!FileSystem::Directory::exists("../RemoteWebPageRepository/" + cat + "/"))
			{
				std::cout << "\n\n --> " << cat << " does not exists in the server repository!!";
				HttpMessage msg = makeMessage(1, "", "localhost::8081");
				msg.addAttribute(HttpMessage::Attribute("IISRet", cat + " does not exists in the server repository!!"));
				sendMessage(msg,si);
				//Send quit message
				HttpMessage quitmsg = makeMessage(1, "quit", "localhost::8081");
				sendMessage(quitmsg, si);
				return;
			}				
			IISPublishHelper(path, cat, cssPath, si);
		}
		catch (std::exception& exc)
		{
			Show::write("\n  Exeception caught: ");
			std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
			Show::write(exMsg);
		}	
}

//IIS Publish helper function
void MsgServer::IISPublishHelper(std::string& path, std::string& cat, std::string& cssPath, Socket& si)
{
	//Create category folder inside IIS Folder
	std::string iisFolderCat = path + "\\" + cat;
	FileSystem::Directory::create(iisFolderCat);

	std::vector<std::string> files = FileSystem::Directory::getFiles("../RemoteWebPageRepository/" + cat + "/");
	for (std::string f : files)
	{
		std::string fileSpec = FileSystem::Path::getFullFileSpec("../RemoteWebPageRepository/" + cat + "/" + f);
		FileSystem::File::copy(fileSpec, iisFolderCat + "\\" + f);
	}

	//Transfer css and js files
	FileSystem::Directory::create(cssPath + "\\ScriptingFiles");

	for (std::string f : FileSystem::Directory::getFiles("../ScriptingFiles/", "*.*"))
	{

		std::string fileSpec = FileSystem::Path::getFullFileSpec("../ScriptingFiles/" + f);
		FileSystem::File::copy(fileSpec, cssPath + "\\ScriptingFiles\\" + f);
	}

	HttpMessage msg = makeMessage(1, "", "localhost::8081");
	msg.addAttribute(HttpMessage::Attribute("IISRet", "IIS Publishing Successful"));
	sendMessage(msg, si);

	//Send quit message
	HttpMessage quitmsg = makeMessage(1, "quit", "localhost::8081");
	sendMessage(quitmsg, si);
}

//returns file names which have no parents
void MsgServer::NoParentFiles(std::string& cat)
{
	try {
			SocketSystem ss;
			SocketConnecter si;
			while (!si.connect("localhost", 8081))
			{
				Show::write("\n server waiting to connect");
				::Sleep(100);
			}
			std::string path = "../RemoteServerRepository/" + cat;
			std::string npf = "";
			Show::stop();
			if (FileSystem::Directory::exists(path))
			{
				NoParentFilesHelper(path, npf);
			}
			else {
				npf = cat + " does not exists";
			}
			//send message back to client
			HttpMessage msg = makeMessage(1, "", "localhost::8081");
			msg.addAttribute(HttpMessage::Attribute("NoParentRet", npf));
			sendMessage(msg, si);

			//Send quit Message
			HttpMessage quitmsg = makeMessage(1, "quit", "localhost::8081");
			sendMessage(quitmsg, si);
		}
		catch (std::exception& exc)
		{
			Show::write("\n  Exeception caught: ");
			std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
			Show::write(exMsg);
		}
}

//No parent files helper
void MsgServer::NoParentFilesHelper(std::string& path, std::string& npf)
{
	char* a[4];
	a[0] = "";
	a[1] = (char*)path.c_str();
	a[2] = "*.h";
	a[3] = "*.cpp";

	CodeAnalysis::CodeAnalysisExecutive exec;
	exec.InitialAnalysis(4, a, exec);
	exec.ExtractFromCommandLine(4, a);
	TypeTable tt;
	ScopeStack<ElementType> s;
	tt.ASTdfs(Repository::getInstance()->getGlobalScope(), s);
	std::cout << "\n--> TypeTable construction completed!!";
	DepAnal da(tt);

	NoSqlDB<std::string> localDb;
	std::vector<std::string> files = FileSystem::Directory::getFiles(path);
	for (std::string file : files)
	{
		localDb = da.dependencyAnalsis(FileSystem::Path::getFullFileSpec(path + "\\" + file));
	}

	//Get no parent files
	std::vector<std::string> noParentFiles = files;

	for (std::string f : files)
	{
		Element<std::string> elem = localDb.RetrieveValueFromDB(FileSystem::Path::getFullFileSpec(path + "\\" + f));
		for (std::string child : elem.children)
		{
			if (std::find(noParentFiles.begin(), noParentFiles.end(), FileSystem::Path::getName(child)) != noParentFiles.end())
			{
				noParentFiles.erase(std::remove(noParentFiles.begin(), noParentFiles.end(), FileSystem::Path::getName(child)), noParentFiles.end());
			}
		}
	}
	for (std::string f : noParentFiles)
	{
		npf += f + ",";
	}
}

//Helper function to demonstrate all req
void MsgServer::MainHelper(HttpMessage& msg)
{	
	if (msg.findValue("Publish") != "")
	{//Message from client to publish the files
		std::string cat = msg.attributes()[msg.findAttribute("Publish")].second;		
		callPublisher(cat);
	}	
	if (msg.findValue("Download") != "")
	{//Message from client to download all files
		std::string cat = msg.attributes()[msg.findAttribute("Download")].second;
		std::cout << "\n\n --> Message received to download all files in category:  " << cat;
		SendHtmFiles(cat);
	}
	if (msg.findValue("GetFiles") != "")
	{	//Message from client to get back all file names from a category
		std::string cat = msg.attributes()[msg.findAttribute("GetFiles")].second;
		GetFileNameOnCat(cat);
	}
	if (msg.findValue("GetFilesLz") != "")
	{	//Message from client to get back all file names from a category for lazy download
		std::string cat = msg.attributes()[msg.findAttribute("GetFilesLz")].second;
		GetFileNameOnCat(cat);
	}
	if (msg.findValue("DownloadSelectedFiles") != "")
	{	//Download Selected Files from server
		std::string csvFiles = msg.attributes()[msg.findAttribute("DownloadSelectedFiles")].second;
		sendSelectedHtmFiles(csvFiles);
	}	
	if (msg.findValue("DownloadLazy") != "")
	{//Lazy Download
		Show::stop();
		std::string x = msg.attributes()[msg.findAttribute("DownloadLazy")].second;
		callPublisher(Utilities::StringHelper::split(x).at(1), true);
		lazyDownload(msg.attributes()[msg.findAttribute("DownloadLazy")].second);
	}	
	if (msg.findValue("Delete") != "")
	{//Delete Selected files from server repository
		DeleteSelectedFiles(msg.attributes()[msg.findAttribute("Delete")].second);
	}	 
	if (msg.findValue("IIS") != "")
	{//IIS publish
		IISPublish(msg.attributes()[msg.findAttribute("IIS")].second);
	}
	if (msg.findValue("NoParent") != "")
	{
		NoParentFiles(msg.attributes()[msg.findAttribute("NoParent")].second);
	}
}
//----< test stub >--------------------------------------------------

int main()
{
  ::SetConsoleTitle(L"HttpMessage Server - Runs Forever");

  Show::attach(&std::cout);
  Show::start();
  Show::title("\n  HttpMessage Server started");

  BlockingQueue<HttpMessage> msgQ;
  MsgServer msgsv;

  try
  {
    SocketSystem ss;
    SocketListener sl(8080, Socket::IP6);
    ClientHandler cp(msgQ);
    sl.start(cp);

    while (true)
    {
      HttpMessage msg = msgQ.deQ();

	  msgsv.MainHelper(msg);

      Show::write("\n\n  server recvd message with body contents:\n" + msg.bodyString());
    }
  }
  catch (std::exception& exc)
  {
    Show::write("\n  Exeception caught: ");
    std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
    Show::write(exMsg);
  }
}