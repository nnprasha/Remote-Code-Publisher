#ifndef WINDOW_H
#define WINDOW_H
///////////////////////////////////////////////////////////////////////////
// Window.h - C++\CLI implementation of WPF Application                  //
//          - Demo for CSE 687 Project #4                                //
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
*  Package Operations:
*  -------------------
*  This package demonstrates how to build a C++\CLI WPF application.  It 
*  provides one class, WPFCppCliDemo, derived from System::Windows::Window
*  that is compiled with the /clr option to run in the Common Language
*  Runtime, and another class MockChannel written in native C++ and compiled
*  as a DLL with no Common Language Runtime support.
*
*  The window class hosts, in its window, a tab control with three views, two
*  of which are provided with functionality that you may need for Project #4.
*  It loads the DLL holding MockChannel.  MockChannel hosts a send queue, a
*  receive queue, and a C++11 thread that reads from the send queue and writes
*  the deQ'd message to the receive queue.
*
*  The Client can post a message to the MockChannel's send queue.  It hosts
*  a receive thread that reads the receive queue and dispatches any message
*  read to a ListBox in the Client's FileList tab.  So this Demo simulates
*  operations you will need to execute for Project #4.
*
*  Public Interfaces:
* ---------------------
*  void setUpStatusBar() - sets the status bar item
*  void setUpTabControl() - manages tab control
*  void setUpSendMessageView() - sets up message view tab
*  void setUpFileListView() - sets up file list view tab
*  void setUpFileListViewHelper1() - file list vew helper
*  void setUpConnectionView() - sets up upload tab
*  void setUpLazyDownloadView() - setup lazy download tab
*  void setUpLazyDownloadViewHelper1(Border^ hBorder3) - lazy download tab helper
*	void setUpLazyDownloadViewHelper2() - lazy download tab helper
*	void setUpIISTab() - sets up IIS Tab

*    void PublishFiles(Object^ obj, RoutedEventArgs^ args) - Setup publish files tab
*    void addText(String^ msg) - adds text to textbox
*	void addItem(String^ msg) - adds item to listbox
*    void getMessage() -gets message from queue
*    void clear(Object^ sender, RoutedEventArgs^ args) - clears all input types
*	void clearDownload(Object^ sender, RoutedEventArgs^ args) - clear download input types

*	void getItemsFromList(Object^ sender, RoutedEventArgs^ args) - gets items from listbox
*    void browseForFolder(Object^ sender, RoutedEventArgs^ args) - browses for folders
*    void OnLoaded(Object^ sender, RoutedEventArgs^ args) - on loaded
*    void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args) - unloading
*	void uploadFilesButton(Object^ sender, RoutedEventArgs^ args) - adds buttons to upload tab
*	void DownloadFiles(Object^ obj, RoutedEventArgs^ args) - on click for download
*	void DisplayCategories(Object^ obj, RoutedEventArgs^ args) - onclick for displaying categories
*	void DisplayCatFiles(Object^ obj, RoutedEventArgs^ args) - onclick for displaying files fia categories
*	void DownloadSelectedFilesFromList(Object^ obj, RoutedEventArgs^ args) - onclick for selected file display
*	void DisplayLazyCatFiles(Object^ obj, RoutedEventArgs^ args) - onclick for lazy download
*	void addItemToLazyTab(String^ msg) - adds item to lazy list box
*	void DownloadSelectedLazyFile(Object^ obj, RoutedEventArgs^ args) - onclick for lazy
*	void DeleteSelectedFiles(Object^ obj, RoutedEventArgs^ args) - - onclick for delete files
*	void StartIIS(Object^ obj, RoutedEventArgs^ args) - IIS Functionality
*	void addToStatusBar(String^ msg) - adds to status bar
*	void NoParentFile(Object^ obj, RoutedEventArgs^ args) - No Parent functionality
*
* BELOW FUNCTIONS ARE TO DISPLAY REQUIREMENTS- 
*	void DemoReq1();
*	void DemoReq2();
*	void DemoReq3();
*	void DemoReq4();
*	void DemoReq5();
*	void DemoReq6();
*	void DemoReq8();
*	void DemoReq9();
*	void DemoReq10();
*
*
*
*  Required Files:
*  ---------------
*  Window.h, Window.cpp, MochChannel.h, MochChannel.cpp,
*  Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*  - this builds C++\CLI client application and native mock channel DLL
*
*  Maintenance History:
*  --------------------
* ver 3.1 : 2nd May 2016
*	- Integrated it with Remote Code Publisher
*
*  ver 3.0 : 22 Apr 2016
*  - added support for multi selection of Listbox items.  Implemented by
*    Saurabh Patel.  Thanks Saurabh.
*  ver 2.0 : 15 Apr 2015
*  - completed message passing demo with moch channel
*  - added FileBrowserDialog to show files in a selected directory
*  ver 1.0 : 13 Apr 2015
*  - incomplete demo with GUI but not connected to mock channel
*/
/*
* Create C++/CLI Console Application
* Right-click project > Properties > Common Language Runtime Support > /clr
* Right-click project > Add > References
*   add references to :
*     System
*     System.Windows.Presentation
*     WindowsBase
*     PresentatioCore
*     PresentationFramework
*/
using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;

#include "../MockChannel/MockChannel.h"
#include <iostream>

namespace CppCliWindows
{
  ref class WPFCppCliDemo : Window
  {
    // MockChannel references

    ISendr* pSendr_;
    IRecvr* pRecvr_;
    IMockChannel* pChann_;

    // Controls for Window

    DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
    Grid^ hGrid = gcnew Grid();                    
    TabControl^ hTabControl = gcnew TabControl();
    TabItem^ hSendMessageTab = gcnew TabItem();
	TabItem^ hDownloadTab = gcnew TabItem();
    TabItem^ hFileListTab = gcnew TabItem();
	TabItem^ hLazyDownloadTab = gcnew TabItem();
	TabItem^ hIISTab = gcnew TabItem();

    StatusBar^ hStatusBar = gcnew StatusBar();
    StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
    TextBlock^ hStatus = gcnew TextBlock();
	RadioButton^ cat1 = gcnew RadioButton();
	RadioButton^ cat2 = gcnew RadioButton();
	RadioButton^ cat3 = gcnew RadioButton();


	RadioButton^ catpub1 = gcnew RadioButton();
	RadioButton^ catpub2 = gcnew RadioButton();
	RadioButton^ catpub3 = gcnew RadioButton();

	RadioButton^ catdow1 = gcnew RadioButton();
	RadioButton^ catdow2 = gcnew RadioButton();
	RadioButton^ catdow3 = gcnew RadioButton();

	RadioButton^ catlaz1 = gcnew RadioButton();
	RadioButton^ catlaz2 = gcnew RadioButton();
	RadioButton^ catlaz3 = gcnew RadioButton();

	RadioButton^ catIIS1 = gcnew RadioButton();
	RadioButton^ catIIS2 = gcnew RadioButton();
	RadioButton^ catIIS3 = gcnew RadioButton();

	StackPanel^ sp = gcnew StackPanel();

    // Controls for SendMessage View

    Grid^ hSendMessageGrid = gcnew Grid();
	Grid^ hDownloadGrid = gcnew Grid();
	Grid^ hLazyDownloadGrid = gcnew Grid();
	Grid^ hIISTabGrid = gcnew Grid();

    Button^ hClearButton = gcnew Button();
	Button^ hClearDownloadButton = gcnew Button();
	Button^ hPublishCatButton = gcnew Button();
	Button^ hDownloadCatButton = gcnew Button();
	Button^ hDisplayCat = gcnew Button();
	Button^ hdisplayCatFiles = gcnew Button();
	Button^ hDownloadSelectedFilesFromList = gcnew Button();
	Button^ hLazyDButton1 = gcnew Button();
	Button^ hLazyDButton2 = gcnew Button();
	Button^ hLazyDButton3 = gcnew Button();
	Button^ hDeleteFileButton = gcnew Button();
	Button^ hNoParentFile = gcnew Button();
	TextBlock^ hTextBlock1 = gcnew TextBlock();
	TextBlock^ hTextBlock2 = gcnew TextBlock();
	TextBlock^ hTextBlock3 = gcnew TextBlock();
	TextBox^ hIISTextBox = gcnew TextBox();

    ScrollViewer^ hScrollViewer1 = gcnew ScrollViewer();
	ScrollViewer^ hScrollViewer2 = gcnew ScrollViewer();
	ScrollViewer^ hScrollViewer3 = gcnew ScrollViewer();

    StackPanel^ hStackPanel1 = gcnew StackPanel();
	StackPanel^ radioStackPanel = gcnew StackPanel();
	StackPanel^ radioStackPubPanel = gcnew StackPanel();
	StackPanel^ hLazyStackPanel1 = gcnew StackPanel();
	StackPanel^ hLazyStackPanel2 = gcnew StackPanel();
    String^ msgText 
      = "Command:ShowMessage\n"   // command
      + "Sendr:localhost@8080\n"  // send address
      + "Recvr:localhost@8090\n"  // receive address
      + "Content-length:44\n"     // body length attribute
      + "\n"                      // end header
      + "Hello World\nCSE687 - Object Oriented Design";  // message body

    // Controls for FileListView View
    Grid^ hFileListGrid = gcnew Grid();
    Forms::FolderBrowserDialog^ hFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
    ListBox^ hListBox = gcnew ListBox();
	ListBox^ hListBox2 = gcnew ListBox();
	ListBox^ hListBox3 = gcnew ListBox();

    Button^ hFolderBrowseButton = gcnew Button();
	Button^ hUploadFilesButton = gcnew Button();
	Button^ hIISButton = gcnew Button();
    Grid^ hGrid2 = gcnew Grid();

    // receive thread

    Thread^ recvThread;

  public:
    WPFCppCliDemo();
    ~WPFCppCliDemo();

	void WPFCppCliDemoHelper();
    void setUpStatusBar();
    void setUpTabControl();
    void setUpSendMessageView();
    void setUpFileListView();
	void setUpFileListViewHelper1();
    void setUpConnectionView();
	void setUpLazyDownloadView();
	void setUpLazyDownloadViewHelper1(Border^ hBorder3);
	void setUpLazyDownloadViewHelper2();
	void setUpIISTab();
	void setIISHelper();
	void onClickRadio(Object^ obj, RoutedEventArgs^ args);

    void PublishFiles(Object^ obj, RoutedEventArgs^ args);
    void addText(String^ msg);
	void addItem(String^ msg);
    void getMessage();
    void clear(Object^ sender, RoutedEventArgs^ args);
	void clearDownload(Object^ sender, RoutedEventArgs^ args);

	void getItemsFromList(Object^ sender, RoutedEventArgs^ args);
    void browseForFolder(Object^ sender, RoutedEventArgs^ args);
    void OnLoaded(Object^ sender, RoutedEventArgs^ args);
    void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
	void uploadFilesButton(Object^ sender, RoutedEventArgs^ args);
	void DownloadFiles(Object^ obj, RoutedEventArgs^ args);
	void DisplayCategories(Object^ obj, RoutedEventArgs^ args);
	void DisplayCatFiles(Object^ obj, RoutedEventArgs^ args);
	void DownloadSelectedFilesFromList(Object^ obj, RoutedEventArgs^ args);
	void DisplayLazyCatFiles(Object^ obj, RoutedEventArgs^ args);
	void addItemToLazyTab(String^ msg);
	void DownloadSelectedLazyFile(Object^ obj, RoutedEventArgs^ args);
	void DeleteSelectedFiles(Object^ obj, RoutedEventArgs^ args);
	void StartIIS(Object^ obj, RoutedEventArgs^ args);
	void addToStatusBar(String^ msg);
	void NoParentFile(Object^ obj, RoutedEventArgs^ args);

	void DemoReq1();
	void DemoReq2();
	void DemoReq3();
	void DemoReq4();
	void DemoReq5();
	void DemoReq6();
	void DemoReq8();
	void DemoReq9();
	void DemoReq10();
  private:

    String^ selCat = "";
    std::string toStdString(String^ pStr);
    String^ toSystemString(std::string& str);
    void setTextBlockProperties();
	void setDownloadTabTextProperties();
    void setButtonsProperties();
	void SetButtonPropertiesHelper1();
	void setDownloadButtonProperties();
	void setDownloadButtonPropertiesHelper1(Border^ hBorder7, Border^ hBorder6, Border^ hBorder8, Border^ hBorder9, TextBlock^ hSpacer);
	void setDownloadButtonPropertiesHelper2();
};
}


#endif
