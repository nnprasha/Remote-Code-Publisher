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

#define IN_DLL

#include "MockChannel.h"
#include "../MsgClient/MsgClient.h"
#include "Cpp11-BlockingQueue.h"
#include <string>
#include <thread>
#include <iostream>
#include <shellapi.h>

using namespace Async;

using BQueue = BlockingQueue < Message >;

/////////////////////////////////////////////////////////////////////////////
// Sendr class
// - accepts messages from client for consumption by MockChannel
//
class Sendr : public ISendr
{
public:
  void postMessage(const Message& msg);
  BQueue& queue();
  
private:
  BQueue sendQ_;
};

void Sendr::postMessage(const Message& msg)
{
  sendQ_.enQ(msg);
}

BQueue& Sendr::queue() { return sendQ_; }

/////////////////////////////////////////////////////////////////////////////
// Recvr class
// - accepts messages from MockChanel for consumption by client
//
class Recvr : public IRecvr
{
public:
  Message getMessage();
  BQueue& queue();
private:
  BQueue recvQ_;
};

Message Recvr::getMessage()
{
  return recvQ_.deQ();
}

BQueue& Recvr::queue()
{
  return recvQ_;
}
/////////////////////////////////////////////////////////////////////////////
// MockChannel class
// - reads messages from Sendr and writes messages to Recvr
//
class MockChannel : public IMockChannel
{
public:
  MockChannel(ISendr* pSendr, IRecvr* pRecvr);
  void start();
  void stop();
  MsgClient c1;
  void Publish(Message& msg, Sendr* pSendr);
  void Download(Message& msg, Sendr* pSendr);
  void GetFiles(Message& msg, Sendr* pSendr);
  void GetFilesLz(Message& msg, Sendr* pSendr);
  void DownloadSelectedFiles(Message& msg, Sendr* pSendr);
  void DownloadLazy(Message& msg, Sendr* pSendr);
  void Upload(Message& msg, Sendr* pSendr);
  void Delete(Message& msg, Sendr* pSendr);
  void IIS(Message& msg, Sendr* pSendr);
  void NoParentFile(Message& msg, Sendr* pSendr);
  void makeDummyIISIndex(std::string& path);
private:
  std::thread thread_;
  ISendr* pISendr_;
  IRecvr* pIRecvr_;
  bool stop_ = false;
};

//----< pass pointers to Sender and Receiver >-------------------------------

MockChannel::MockChannel(ISendr* pSendr, IRecvr* pRecvr) : pISendr_(pSendr), pIRecvr_(pRecvr) {}

//Publish message to server
void MockChannel::Publish(Message& msg, Sendr* pSendr)
{
	std::thread t1(
		[&]()
	{
		std::string x = c1.PublishFiles(msg);
		std::cout << " \n\n --> " << x;
		pSendr->postMessage(x);
	} // 20 messages 100 millisec apart
	);
	t1.join();
}

//Download message to server
void MockChannel::Download(Message& msg, Sendr* pSendr)
{
	std::thread t1(
		[&]() {
		std::string x = c1.DownloadFiles(msg);
		pSendr->postMessage(x);
	} // 20 messages 100 millisec apart
	);
	t1.join();
}

void MockChannel::GetFiles(Message& msg, Sendr* pSendr)
{
	std::thread t1(
		[&]() {
		std::string x = c1.GetFiles(msg);
		pSendr->postMessage(x);
	}
	);
	t1.join();
}

//Get lazy files from server
void MockChannel::GetFilesLz(Message& msg, Sendr* pSendr)
{
	std::thread t1(
		[&]() {
		std::string x = c1.GetFiles(msg);
		pSendr->postMessage(x);
	}
	);
	t1.join();
}

//Download Selected files from server
void MockChannel::DownloadSelectedFiles(Message& msg, Sendr* pSendr)
{
	std::thread t1(
		[&]() {
		c1.GetSelectedHtmFiles(msg);
		pSendr->postMessage("Files downloaded in the \nLocalClientWebPageRepository/SelectedFilesDownloadFolder/");

		std::string d = Utilities::StringHelper::split(msg).at(2);
		d = "..\\LocalClientWebPageRepository\\SelectedFilesDownloadFolder\\" + d + ".htm";
		std::wstring wideURL;
		for (size_t i = 0; i < d.length(); ++i)
			wideURL += wchar_t(d[i]);

		const wchar_t* url = wideURL.c_str();
		ShellExecute(NULL, NULL, url, NULL, NULL, SW_SHOWNORMAL);
	}
	);
	t1.join();
}

//Lazy download message to server
void MockChannel::DownloadLazy(Message& msg, Sendr* pSendr)
{
	std::thread t1(
		[&]() {

		std::string x = c1.LazyDownload(msg);
		pSendr->postMessage("Files downloaded in the \nLocalClientWebPageRepository/" + x + "/ Folder.");


		std::string d = Utilities::StringHelper::split(msg).at(2);
		pSendr->postMessage("Opening " + d + ".htm ...");
		d = "..\\LocalClientWebPageRepository\\" + x + "\\" + d + ".htm";
		std::wstring wideURL;
		for (size_t i = 0; i < d.length(); ++i)
			wideURL += wchar_t(d[i]);

		const wchar_t* url = wideURL.c_str();
		ShellExecute(NULL, NULL, url, NULL, NULL, SW_SHOWNORMAL);
	}
	);
	t1.join();
}

//Upload message to server
void MockChannel::Upload(Message& msg, Sendr* pSendr)
{
	std::vector<std::string> splitString = Utilities::StringHelper::split(msg);
	std::thread t1(
		[&]() { c1.UploadFiles(splitString.at(1), splitString.at(2)); } // 20 messages 100 millisec apart
	);
	t1.join();
}

//Delete message to server
void MockChannel::Delete(Message& msg, Sendr* pSendr)
{
	std::thread t1(
		[&]() {
		std::string x = c1.DeleteSelectedFiles(msg);
		pSendr->postMessage("Deleted Files: \n" + x);
		pSendr->postMessage("../RemoteWebPageRepository/" + Utilities::StringHelper::split(msg).at(1) + "can be checked to verify");
	}

	);
	t1.join();
}

//Get NO parent files from server
void MockChannel::NoParentFile(Message& msg, Sendr* pSendr)
{
	std::thread t1(
		[&]() {
		std::string x = c1.NoParentFile(msg);
		if(x.find(',') != -1)
			x = "gtf," + x;		
		pSendr->postMessage(x);
	}

	);
	t1.join();
}

//Send IIS Publish message to server
void MockChannel::IIS(Message& msg, Sendr* pSendr)
{
	std::thread t1(
		[&]() {
		std::string x = c1.IISPublish(msg);
		x = "IISRet," + x;
		pSendr->postMessage(x);
		makeDummyIISIndex(Utilities::StringHelper::split(msg).at(2));

		std::string open = "start chrome @localhost:8080//";
		std::system(open.c_str());
	}
	);
	t1.join();
}

//makes dummy iis page
void MockChannel::makeDummyIISIndex(std::string& path)
{
	std::FILE* myfile;
	path = path + "\\index.htm";
	fopen_s(&myfile, path.c_str(), "w+");
	if (myfile == NULL)
		throw "Invalid Directory Path!!";
	std::string fContent = "<html><body><p>";
	fContent += "<br /> Please enter in the URL BAR : localhost:8080/name_of_the_category_that_has_been_published";
	fContent += "<br /> for eg: localhost:8080/Category1  or  localhost:8080/Category2  or  localhost:8080/Category3";
	fContent += "</p></body></html>";
	fprintf(myfile, fContent.c_str());
	fclose(myfile);
}

//----< creates thread to read from sendQ and echo back to the recvQ >-------
void MockChannel::start()
{
  std::cout << "\n \n ---> MockChannel starting up";
  thread_ = std::thread(
    [this] {
    Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
    Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
    if (pSendr == nullptr || pRecvr == nullptr)
    {
      std::cout << "\n  failed to start Mock Channel\n\n";
      return;
    }
    BQueue& sendQ = pSendr->queue();
    BQueue& recvQ = pRecvr->queue();
    while (!stop_)
    {
      //std::cout << "\n  channel deQing message";
      Message msg = sendQ.deQ();  // will block here so send quit message when stopping
      //std::cout << "\n  channel enQing message";
      recvQ.enQ(msg);		
	  if (Utilities::StringHelper::split(msg).at(0) == "Publish")
		  Publish(msg, pSendr);
	  else if (Utilities::StringHelper::split(msg).at(0) == "Download")
		  Download(msg, pSendr);
	  else if (Utilities::StringHelper::split(msg).at(0) == "GetFiles")
		  GetFiles(msg, pSendr);
	  else if (Utilities::StringHelper::split(msg).at(0) == "GetFilesLz")
		  GetFilesLz(msg, pSendr);
	  else if (Utilities::StringHelper::split(msg).at(0) == "DownloadSelectedFiles")
		  DownloadSelectedFiles(msg, pSendr);
	  else if (Utilities::StringHelper::split(msg).at(0) == "DownloadLazy")
		  DownloadLazy(msg, pSendr);
	  else if (Utilities::StringHelper::split(msg).at(0) == "Upload")
		  Upload(msg, pSendr);
	  else if (Utilities::StringHelper::split(msg).at(0) == "Delete")
		  Delete(msg, pSendr);
	  else if (Utilities::StringHelper::split(msg).at(0) == "IIS")
		  IIS(msg, pSendr);
	  else if (Utilities::StringHelper::split(msg).at(0) == "NoParent")
		  NoParentFile(msg, pSendr);
    }
    std::cout << "\n  Server stopping\n\n";
  });
}

//----< signal server thread to stop >---------------------------------------

void MockChannel::stop() { stop_ = true; }

//----< factory functions >--------------------------------------------------

ISendr* ObjectFactory::createSendr() { return new Sendr; }

IRecvr* ObjectFactory::createRecvr() { return new Recvr; }

IMockChannel* ObjectFactory::createMockChannel(ISendr* pISendr, IRecvr* pIRecvr) 
{ 
  return new MockChannel(pISendr, pIRecvr); 
}

#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
  ObjectFactory objFact;
  ISendr* pSendr = objFact.createSendr();
  IRecvr* pRecvr = objFact.createRecvr();
  IMockChannel* pMockChannel = objFact.createMockChannel(pSendr, pRecvr);
  pMockChannel->start();
  pSendr->postMessage("Hello World");
  pSendr->postMessage("CSE687 - Object Oriented Design");
  Message msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pSendr->postMessage("stopping");
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pMockChannel->stop();
  pSendr->postMessage("quit");
  std::cin.get();
}
#endif
