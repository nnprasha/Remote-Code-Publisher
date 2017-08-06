//////////////////////////////////////////////////////////////////////////////
// MsgClient.cpp - Demonstrates connection from clint to server				//
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
 * This package implements a client that sends HTTP style messages and
 * files to a server that simply displays messages and stores files.
 *
 */
/*
* Public Interface:
* --------------------
*
* ServerHandler(BlockingQueue<HttpMessage>& msgQ) - Constructor that initializes a blocking queue
* void operator()(Socket socket) - Operator that runs on a thread to read messages from the server
* void ProcessFileMsg(Socket& socket, HttpMessage& msg, std::string& filename) - Helper function to process file messages
* HttpMessage readMessage(Socket& socket) - reads http message from server
* bool readFile(const std::string& filename, size_t fileSize, Socket& socket, std::string& cat) - reads files from the server.
* 
* Required Files:
* ---------------
*   MsgClient.cpp, MsgServer.cpp
*   HttpMessage.h, HttpMessage.cpp
*   Cpp11-BlockingQueue.h
*   Sockets.h, Sockets.cpp
*   FileSystem.h, FileSystem.cpp
*   Logger.h, Logger.cpp
*   Utilities.h, Utilities.cpp
*/


#include "MsgClient.h"
#include <string>
#include <iostream>
#include <thread>
#include <shellapi.h>

using namespace Logging;
using Show = StaticLogger<1>;
using namespace Utilities;
using Utils = StringHelper;
using namespace Async;

static bool responseEnded = false;
static std::string getFilesOnCat = "";
static std::string lazyFolder = "";
static std::string noFilesPresent = "";
static std::string publishMessage = "";
static std::string filesDeleted = "";
static std::string iisRet = "";
static std::string noParentRet = "";
/////////////////////////////////////////////////////////////////////
// ClientCounter creates a sequential number for each client
//
class ClientCounter
{
public:
  ClientCounter() { ++clientCount; }
  size_t count() { return clientCount; }
private:
  static size_t clientCount;
};

size_t ClientCounter::clientCount = 0;

class ServerHandler
{
public:
	using EndPoint = std::string;
	ServerHandler(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
	void operator()(Socket socket);	
	void ProcessFileMsg(Socket& socket, HttpMessage& msg, std::string& filename);

private:
	bool connectionClosed_;
	HttpMessage readMessage(Socket& socket);
	bool readFile(const std::string& filename, size_t fileSize, Socket& socket, std::string& cat);
	BlockingQueue<HttpMessage>& msgQ_;
};

//Operator that reads messages and runs on seperate thread
void ServerHandler::operator()(Socket socket)
{
	while (true)
	{
		HttpMessage msg = readMessage(socket);

		if (msg.findValue("GetFiles") != "")
		{
			getFilesOnCat = msg.attributes()[msg.findAttribute("GetFiles")].second;
		}

		if (msg.findValue("PublishRetMessage") != "")
		{
			publishMessage = msg.attributes()[msg.findAttribute("PublishRetMessage")].second;
		}

		if (msg.findValue("NoFilesPresent") != "")
		{
			noFilesPresent = msg.attributes()[msg.findAttribute("NoFilesPresent")].second;
		}

		if (msg.findValue("LazyDownload") != "")
		{
			lazyFolder = msg.attributes()[msg.findAttribute("Category")].second;
		}

		if (msg.findValue("DeletedFiles") != "")
		{
			filesDeleted = msg.attributes()[msg.findAttribute("DeletedFiles")].second;
		}

		if (msg.findValue("IISRet") != "")
		{
			iisRet = msg.attributes()[msg.findAttribute("IISRet")].second;
		}
		if (msg.findValue("NoParentRet") != "")
		{
			noParentRet = msg.attributes()[msg.findAttribute("NoParentRet")].second;
		}
		if (connectionClosed_ || msg.bodyString() == "quit")
		{
			Show::write("\n\n  clienthandler thread is terminating");
			responseEnded = true;
			break;
		}
		msgQ_.enQ(msg);
	}
}

//Helper function for read message
void ServerHandler::ProcessFileMsg(Socket& socket, HttpMessage& msg, std::string& filename)
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

//Read message sent from server
HttpMessage ServerHandler::readMessage(Socket& socket)
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
			std::string cat = msg.attributes()[msg.findAttribute("Category")].second;
			readFile(filename, contentSize, socket, cat);
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
bool ServerHandler::readFile(const std::string& filename, size_t fileSize, Socket& socket, std::string& cat)
{

	if (!FileSystem::Directory::exists("../LocalClientWebPageRepository/" + cat))
		FileSystem::Directory::create("../LocalClientWebPageRepository/" + cat);

	std::cout << "\n\n --> Receiving file from server: " << filename << " (** REQUIREMENT 5)";
	std::string fqname = "../LocalClientWebPageRepository/" + cat + "/" + filename;
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


//----< factory for creating messages >------------------------------
/*
 * This function only creates one type of message for this demo.
 * - To do that the first argument is 1, e.g., index for the type of message to create.
 * - The body may be an empty string.
 * - EndPoints are strings of the form ip:port, e.g., localhost:8081. This argument
 *   expects the receiver EndPoint for the toAddr attribute.
 */
HttpMessage MsgClient::makeMessage(size_t n, const std::string& body, const EndPoint& ep)
{
  HttpMessage msg;
  HttpMessage::Attribute attrib;
  EndPoint myEndPoint = "localhost:8081";  // ToDo: make this a member of the sender
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
//----< send message using socket >----------------------------------

void MsgClient::sendMessage(HttpMessage& msg, Socket& socket)
{
  std::string msgString = msg.toString();
  bool hi=socket.send(msgString.size(), (Socket::byte*)msgString.c_str());
}
//----< send file using socket >-------------------------------------
/*
 * - Sends a message to tell receiver a file is coming.
 * - Then sends a stream of bytes until the entire file
 *   has been sent.
 * - Sends in binary mode which works for either text or binary.
 */
bool MsgClient::sendFile(const std::string& filename, Socket& socket,std::string& category)
{
  // assumes that socket is connected

 // std::string fqname = "../TestFiles/" + filename;
  FileSystem::FileInfo fi(filename);
  size_t fileSize = fi.size();
  std::string sizeString = Converter<size_t>::toString(fileSize);
  FileSystem::File file(filename);
  file.open(FileSystem::File::in, FileSystem::File::binary);
  if (!file.isGood())
    return false;
  
  HttpMessage msg = makeMessage(1, "", "localhost::8080");
  msg.addAttribute(HttpMessage::Attribute("file", filename));
  msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
  msg.addAttribute(HttpMessage::Attribute("Category", category));
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

//send publish files message to server
std::string MsgClient::PublishFiles(std::string& msg)
{
	try {

		//Send Publish Message to server
		HttpMessage httpmsg = makeMessage(1, "", "localhost::8080");
		std::vector<std::string> pubcat = Utilities::StringHelper::split(msg);
		httpmsg.addAttribute(HttpMessage::Attribute(pubcat.at(0), pubcat.at(1)));
		sendMessage(httpmsg, si);


		//Start Client Listener
		BlockingQueue<HttpMessage> msgQ;
		SocketListener sl(8081, Socket::IP6);
		ServerHandler sp(msgQ);
		sl.start(sp);

		while (true)
		{
			if (responseEnded)
				break;
		}
		responseEnded = false;
		sl.close();

		std::string ret = publishMessage;
		publishMessage = "";
		return ret;
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}	
	return "Exception thrown";
}

//Clinet constructor, connecting to the server
MsgClient::MsgClient()
{
	try {
		while (!si.connect("localhost", 8080))
		{
			Show::write("\n client waiting to connect");
			::Sleep(100);
		}
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}	
}

//Send message to download files from the server
std::string MsgClient::DownloadFiles(std::string& msg)
{
	//Send Download Message to server
	HttpMessage httpmsg = makeMessage(1, "", "localhost::8080");
	httpmsg.addAttribute(HttpMessage::Attribute(Utilities::StringHelper::split(msg).at(0),
		Utilities::StringHelper::split(msg).at(1)));

	sendMessage(httpmsg, si);

	BlockingQueue<HttpMessage> msgQ;
	SocketListener sl(8081, Socket::IP6);
	ServerHandler sp(msgQ);
	sl.start(sp);

	//Start Client Listener
	while (true)
	{
		if (responseEnded)
			break;
	}

	responseEnded = false;
	sl.close();

	if (noFilesPresent != "") {
		std::string x = "No files present to download in " + noFilesPresent;
		noFilesPresent = "";		
		return x;
	}
	
	openStartUpPage("../LocalClientWebPageRepository/", Utilities::StringHelper::split(msg).at(1));
	return "Files downloaded to LocalClientWebPageRepository/"+ Utilities::StringHelper::split(msg).at(1);	
}

//Opening the startup webpage
void MsgClient::openStartUpPage(std::string webPageRepoPath,std::string cat)
{
	std::cout << "\n\n--> Opening index.htm...";
	
	std::string d = "";
	for (size_t i = 0; i < webPageRepoPath.length(); i++)
	{
		if (webPageRepoPath[i] == '/')
			d += "\\";
		else
			d += webPageRepoPath[i];
	}
	d += cat+"\\index.htm";

	//using Shell Execute to open the Startup webpage file
	std::wstring wideURL;
	for (size_t i = 0; i < d.length(); ++i)
		wideURL += wchar_t(d[i]);

	const wchar_t* url = wideURL.c_str();
	ShellExecute(NULL, NULL, url, NULL, NULL, SW_SHOWNORMAL);	
}

//Send message to get source files from the server
std::string& MsgClient::GetFiles(std::string& msg)
{
	try {
		//Send Get Files message to server
		HttpMessage getFilesmsg = makeMessage(1, "", "localhost::8080");
		getFilesmsg.addAttribute(HttpMessage::Attribute(Utilities::StringHelper::split(msg).at(0),
			Utilities::StringHelper::split(msg).at(1)));

		sendMessage(getFilesmsg, si);


		//Start Client Listener
		BlockingQueue<HttpMessage> msgQ;
		SocketListener sl(8081, Socket::IP6);
		ServerHandler sp(msgQ);
		sl.start(sp);

		while (true)
		{
			if (responseEnded)
				break;
		}
		responseEnded = false;
		sl.close();
		if(Utilities::StringHelper::split(msg).at(0) == "GetFiles"
			&& getFilesOnCat.find(',') != -1)
			getFilesOnCat = "gtf," + getFilesOnCat;
		else if(Utilities::StringHelper::split(msg).at(0) == "GetFilesLz"
			&& getFilesOnCat.find(',') != -1)
			getFilesOnCat = "gtflz," + getFilesOnCat;
		return getFilesOnCat;
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
	return getFilesOnCat;
}

//Delete selected files from the server repo
std::string MsgClient::DeleteSelectedFiles(std::string& msg)
{
	try {
		HttpMessage delMsg = makeMessage(1, "", "localhost::8080");
		delMsg.addAttribute(HttpMessage::Attribute(Utilities::StringHelper::split(msg).at(0), msg));
		sendMessage(delMsg, si);

		//Start Client Listener
		BlockingQueue<HttpMessage> msgQ;
		SocketListener sl(8081, Socket::IP6);
		ServerHandler sp(msgQ);
		sl.start(sp);

		while (true)
		{
			if (responseEnded)
				break;
		}
		responseEnded = false;
		sl.close();
		std::string x = filesDeleted;
		filesDeleted = "";
		return x;
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
	return "Exception thrown";
}

//Lazy download from the server
std::string MsgClient::LazyDownload(std::string& msg)
{
	try {
		//Send lazy download Files message to server
		HttpMessage getFilesmsg = makeMessage(1, "", "localhost::8080");
		getFilesmsg.addAttribute(HttpMessage::Attribute(Utilities::StringHelper::split(msg).at(0),
			msg));

		sendMessage(getFilesmsg, si);


		//Start Client Listener
		BlockingQueue<HttpMessage> msgQ;
		SocketListener sl(8081, Socket::IP6);
		ServerHandler sp(msgQ);
		sl.start(sp);

		while (true)
		{
			if (responseEnded)
				break;
		}
		responseEnded = false;
		sl.close();
		std::string x = lazyFolder;
		lazyFolder = "";
		return x;
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
	return "Exception thrown";
}

//IIS Publishing
std::string MsgClient::IISPublish(std::string msg)
{
	try {
		HttpMessage iisMsg = makeMessage(1, "", "localhost::8080");
		iisMsg.addAttribute(HttpMessage::Attribute("IIS", msg));
		sendMessage(iisMsg, si);


		BlockingQueue<HttpMessage> msgQ;
		SocketListener sl(8081, Socket::IP6);
		ServerHandler sp(msgQ);
		sl.start(sp);

		//Start Client Listener
		while (true)
		{
			if (responseEnded)
				break;
		}
		responseEnded = false;
		sl.close();

		std::string x = iisRet;
		iisRet = "";
		return x;
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
	return "Exception thrown";
}

//Uploading source code files to the server repo
void MsgClient::UploadFiles(std::string& path,std::string& category)
{
  
  try
  {       
	  std::cout << "\n\n --> Uploading files to category: " << category <<"(** REQUIREMENT 5)";
    std::vector<std::string> files = FileSystem::Directory::getFiles(path, "*.cpp");
    for (size_t i = 0; i < files.size(); ++i)
    {
      std::cout <<"\n\t -->  sending file: "<<files[i];
      sendFile(FileSystem::Path::getFullFileSpec(path+"\\"+files[i]), si, category);
    }

	files = FileSystem::Directory::getFiles(path, "*.h");
	for (size_t i = 0; i < files.size(); i++)
	{
		std::cout << "\n\t -->  sending file: " << files[i];
		sendFile(path+"\\"+files[i], si, category);
	}
	
  }
  catch (std::exception& exc)
  {
    Show::write("\n  Exeception caught: ");
    std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
    Show::write(exMsg);
  }
}

//Get no parent file from server
std::string MsgClient::NoParentFile(std::string& msg)
{
	try {
		//Send Get Files message to server
		HttpMessage getFilesmsg = makeMessage(1, "", "localhost::8080");
		getFilesmsg.addAttribute(HttpMessage::Attribute(Utilities::StringHelper::split(msg).at(0),
			Utilities::StringHelper::split(msg).at(1)));

		sendMessage(getFilesmsg, si);


		//Start Client Listener
		BlockingQueue<HttpMessage> msgQ;
		SocketListener sl(8081, Socket::IP6);
		ServerHandler sp(msgQ);
		sl.start(sp);

		while (true)
		{
			if (responseEnded)
				break;
		}
		responseEnded = false;
		sl.close();
		std::string x = noParentRet;
		noParentRet = "";
		return x;
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
	return "Exception thrown";
}

//download selected htm files from server
void MsgClient::GetSelectedHtmFiles(std::string& msg)
{
	try {
		//Send Get Files message to server
		HttpMessage getFilesmsg = makeMessage(1, "", "localhost::8080");
		getFilesmsg.addAttribute(HttpMessage::Attribute(Utilities::StringHelper::split(msg).at(0),
			msg));

		sendMessage(getFilesmsg, si);


		BlockingQueue<HttpMessage> msgQ;
		SocketListener sl(8081, Socket::IP6);
		ServerHandler sp(msgQ);
		sl.start(sp);

		//Start Client Listener
		while (true)
		{
			if (responseEnded)
				break;
		}
		responseEnded = false;
		sl.close();
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

//<---- Client test stub ------>

#ifdef TESTING_CLIENT

int main()
{
  ::SetConsoleTitle(L"Clients Running on Threads");

  Show::title("Demonstrating two HttpMessage Clients each running on a child thread");
  
  

  MsgClient c1;
  std::thread t1(
    [&]() { c1.getSelectedHtmFiles("GetFiles"); } 
  );

  MsgClient c2;
  std::thread t2(
    [&]() { c2.getSelectedHtmFiles("GetFiles"); } 
  );
  t1.join();
  t2.join();
}

#endif