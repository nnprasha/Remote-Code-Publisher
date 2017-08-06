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

#include "Window.h"
#include "../Utilities/Utilities.h"
using namespace CppCliWindows;
WPFCppCliDemo::WPFCppCliDemo()
{
	DemoReq1();
	DemoReq2();
	DemoReq3();
	DemoReq4();
	DemoReq5();
	DemoReq6();
	DemoReq8();
	DemoReq9();
	DemoReq10();
  // set up channel
  ObjectFactory* pObjFact = new ObjectFactory;
  pSendr_ = pObjFact->createSendr();
  pRecvr_ = pObjFact->createRecvr();
  pChann_ = pObjFact->createMockChannel(pSendr_, pRecvr_);
  pChann_->start();
  delete pObjFact;
  // client's receive thread
  recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
  recvThread->Start();
  WPFCppCliDemoHelper();
  // set Window properties
  this->Title = "WPF C++/CLI Demo";
  this->Width = 800;
  this->Height = 600;
  // attach dock panel to Window
  this->Content = hDockPanel;
  hDockPanel->Children->Add(hStatusBar);
  hDockPanel->SetDock(hStatusBar, Dock::Bottom);
  hDockPanel->Children->Add(hGrid);
  // setup Window controls and views
  setUpTabControl();
  setUpStatusBar();
  setUpSendMessageView();
  setUpFileListView();
  setUpConnectionView();
  setUpLazyDownloadView();
  setUpIISTab();
}

void WPFCppCliDemo::WPFCppCliDemoHelper()
{
	// set event handlers
	this->Loaded +=
		gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
	this->Closing +=
		gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
	hPublishCatButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::PublishFiles);
	hDownloadCatButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::DownloadFiles);
	hClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clear);
	hClearDownloadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clearDownload);
	hFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder);
	hUploadFilesButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::uploadFilesButton);
	hDisplayCat->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::DisplayCategories);
	hdisplayCatFiles->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::DisplayCatFiles);
	hDownloadSelectedFilesFromList->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::DownloadSelectedFilesFromList);
	hLazyDButton1->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::DisplayLazyCatFiles);
	hLazyDButton2->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::DownloadSelectedLazyFile);
	hLazyDButton3->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clearDownload);
	hDeleteFileButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::DeleteSelectedFiles);
	hIISButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::StartIIS);
	hNoParentFile->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::NoParentFile);
	catdow1->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::onClickRadio);
	catdow2->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::onClickRadio);
	catdow3->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::onClickRadio);

}

void WPFCppCliDemo::onClickRadio(Object^ obj, RoutedEventArgs^ args)
{
	hListBox->Items->Clear();
	hListBox2->Items->Clear();
	hListBox3->Items->Clear();
}

WPFCppCliDemo::~WPFCppCliDemo()
{
  delete pChann_;
  delete pSendr_;
  delete pRecvr_;
}

void WPFCppCliDemo::DemoReq1()
{
	std::cout << "\n\n==============================================\n";
	std::cout << "\tREQUIREMENT NUMBER 1";
	std::cout << "\n==============================================\n\n";
	std::cout << " -->The entire project has been built on Visual Studio 2015 using C++ windows Console.\n  The Project can be checked to verify this requirement.";
}

void WPFCppCliDemo::DemoReq2()
{
	std::cout << "\n\n==============================================\n";
	std::cout << "\tREQUIREMENT NUMBER 2";
	std::cout << "\n==============================================\n\n";
	std::cout << " -->C++ standard libraries have been used in the entire project.\n   The project can be checked to verify this!!";
}

void WPFCppCliDemo::DemoReq3()
{
	std::cout << "\n\n==============================================\n";
	std::cout << "\tREQUIREMENT NUMBER 3";
	std::cout << "\n==============================================\n\n";

	std::cout << " --> CodePublisher Project provides the functionality to publish source code files as linked web pages.";
	std::cout << " \n The project structure can be checked to verify this";
}

void WPFCppCliDemo::DemoReq4()
{

	std::cout << "\n\n==============================================\n";
	std::cout << "\tREQUIREMENT NUMBER 4";
	std::cout << "\n==============================================\n\n";

	std::cout << "All requirement of the code publisher have been satisfied.";
	std::cout << "\n This can be verified by processing the \"Publish Tab\" on the GUI";

}

void WPFCppCliDemo::DemoReq5()
{
	std::cout << "\n\n==============================================\n";
	std::cout << "\tREQUIREMENT NUMBER 5";
	std::cout << "\n==============================================\n\n";

	std::cout << "\n--> The Client can Upload files from the \"Upload Tab\" from the GUI.";
	std::cout << "\n--> The Client can view Repository contents by processing the \"Download Tab\" on the GUI.";
	std::cout << "\n --> All requirements provided in the purpose section can be verified by clicking appropriate buttons \n on the GUI. ";
	
}

void WPFCppCliDemo::DemoReq6()
{
	std::cout << "\n\n==============================================\n";
	std::cout << "\tREQUIREMENT NUMBER 6 and 7";
	std::cout << "\n==============================================\n\n";

	std::cout << "\n --> A message passing communication system is provided. The following projects can be viewed to verify this!";
	std::cout << "\n\t --> Sockets, MsgClient, MsgServer, CppCli-WPF-App";
}

void WPFCppCliDemo::DemoReq8()
{
	std::cout << "\n\n==============================================\n";
	std::cout << "\tREQUIREMENT NUMBER 8";
	std::cout << "\n==============================================\n\n";

	std::cout << "\n Stream of bytes are being sent for file transfer from Client to Server and vice versa. ";
	std::cout << "\n readFile() and sendFile() functions in MsgClient and MsgServer project can be checked to verify this!!";
}

void WPFCppCliDemo::DemoReq9()
{
	std::cout << "\n\n==============================================\n";
	std::cout << "\tREQUIREMENT NUMBER 9";
	std::cout << "\n==============================================\n\n";
	std::cout << "\n An automated unit test suite has been included. \n WPFCppCliDemo class is the test executive for this project!!";
}

void WPFCppCliDemo::DemoReq10()
{

	std::cout << "\n\n==============================================\n";
	std::cout << "\tREQUIREMENT NUMBER 10";
	std::cout << "\n==============================================\n\n";
	std::cout << "\nLazy download functionality can be verified from the \"LazyDownload Tab\" on the GUI. \n\n";
	
}

void WPFCppCliDemo::setUpStatusBar()
{
  hStatusBar->Items->Add(hStatusBarItem);
  hStatus->Text = "very important messages will appear here";
  //status->FontWeight = FontWeights::Bold;
  hStatusBarItem->Content = hStatus;
  hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

void WPFCppCliDemo::setUpTabControl()
{
  hGrid->Children->Add(hTabControl);
  hSendMessageTab->Header = "Publish";
  hFileListTab->Header = "Upload";
  hDownloadTab->Header = "Download/Delete"; 
  hLazyDownloadTab->Header = "Lazy Download";
  hIISTab->Header = "IIS";
  hTabControl->Items->Add(hFileListTab);
  hTabControl->Items->Add(hSendMessageTab);
  hTabControl->Items->Add(hDownloadTab);
  hTabControl->Items->Add(hLazyDownloadTab);
  hTabControl->Items->Add(hIISTab);
}

void WPFCppCliDemo::setTextBlockProperties()
{
  RowDefinition^ hRow1Def = gcnew RowDefinition();
  hSendMessageGrid->RowDefinitions->Add(hRow1Def);

  Border^ hBorder1 = gcnew Border();
  hBorder1->BorderThickness = Thickness(1);
  hBorder1->BorderBrush = Brushes::Black;
  hBorder1->Child = hTextBlock1;
  hTextBlock1->Padding = Thickness(15);
  hTextBlock1->Text = "";
  hTextBlock1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
  hTextBlock1->FontWeight = FontWeights::Bold;
  hTextBlock1->FontSize = 16;

  hScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
  hScrollViewer1->HorizontalScrollBarVisibility = ScrollBarVisibility::Auto;

  hScrollViewer1->Content = hBorder1;
  hScrollViewer1->ScrollToEnd();
  hSendMessageGrid->SetRow(hScrollViewer1, 0);
  hSendMessageGrid->Children->Add(hScrollViewer1);
}

void WPFCppCliDemo::setUpIISTab()
{
	hIISTab->Content = hIISTabGrid;
	TextBlock^ label1 = gcnew TextBlock();
	label1->Text = "Please enter the IIS Folder path (Absolute path), as specified in IIS " +
		"\n Please select which category to publish in IIS.\n";
	label1->FontWeight = FontWeights::Bold;
	TextBlock^ label2 = gcnew TextBlock();
	label2->Text = "Also, when setting up IIS, please specify the port number as 8080, as it will \n automatically open the browser on clicking the button."
		+ " If not set as 8080,\n you will have to manually open it in google chrome by \n specifying \"localhost:port_number\"";
	label2->FontWeight = FontWeights::Bold;
	Border^ hBorder2 = gcnew Border();
	hBorder2->BorderThickness = Thickness(0);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Width = 400;
	hBorder2->Height = 50;
	hBorder2->Child = label1;
	Border^ hBorder4 = gcnew Border();
	hBorder4->BorderThickness = Thickness(0);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Width = 400;
	hBorder4->Height = 100;
	hBorder4->Child = label2;
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Width = 400;
	hBorder1->Height = 50;
	hBorder1->Padding = Thickness(5);
	hBorder1->Child = hIISTextBox;
	hIISTextBox->Text = "";
	hIISTextBox->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hIISTextBox->FontWeight = FontWeights::Bold;
	hIISTextBox->FontSize = 16;
	sp->Orientation = Orientation::Vertical;
	sp->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	sp->Children->Add(hBorder2);
	sp->Children->Add(hBorder4);
	sp->Children->Add(hBorder1);
	hIISButton->Content = "Start IIS";	
	setIISHelper();
}

void WPFCppCliDemo::setIISHelper()
{
	Border^ hBorder3 = gcnew Border();
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Width = 100;
	hBorder3->Height = 30;
	hBorder3->Margin = Thickness(20);
	hBorder3->Child = hIISButton;
	sp->Children->Add(hBorder3);
	catIIS1->Content = "Category1";
	catIIS2->Content = "Category2";
	catIIS3->Content = "Category3";
	sp->Children->Add(catIIS1);
	sp->Children->Add(catIIS2);
	sp->Children->Add(catIIS3);
	catIIS1->IsChecked = "true";
	hIISTabGrid->Children->Add(sp);
}

void WPFCppCliDemo::setDownloadTabTextProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hDownloadGrid->RowDefinitions->Add(hRow1Def);	

	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->Width = 700;		
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hTextBlock2;
	hTextBlock2->Padding = Thickness(1);
	hTextBlock2->Text = "";
	hTextBlock2->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBlock2->FontWeight = FontWeights::Bold;
	hTextBlock2->FontSize = 16;
	hScrollViewer2->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer2->HorizontalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer2->Content = hBorder1;
	hScrollViewer2->ScrollToEnd();
	hDownloadGrid->SetRow(hScrollViewer2, 0);
	hDownloadGrid->Children->Add(hScrollViewer2);

	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hDownloadGrid->RowDefinitions->Add(hRow2Def);
	Border^ hBorder2 = gcnew Border();
	hBorder2->Margin = Thickness(10);
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->Width = 700;
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hListBox2;

	hListBox2->SelectionMode = SelectionMode::Multiple;
	hDownloadGrid->SetRow(hBorder2, 1);
	hDownloadGrid->Children->Add(hBorder2);
}


void WPFCppCliDemo::setButtonsProperties()
{
  RowDefinition^ hRow2Def = gcnew RowDefinition();
  hRow2Def->Height = GridLength(75);
  hSendMessageGrid->RowDefinitions->Add(hRow2Def);

  RowDefinition^ hRow3Def = gcnew RowDefinition();
  hRow3Def->Height = GridLength(75);
  hSendMessageGrid->RowDefinitions->Add(hRow3Def);

  hPublishCatButton->Content = "Publish Selected Category";
  Border^ hBorder7 = gcnew Border();
  hBorder7->Width = 200;
  hBorder7->Height = 30;
  hBorder7->BorderThickness = Thickness(1);
  hBorder7->BorderBrush = Brushes::Black;
  hBorder7->Child = hPublishCatButton;

  hClearButton->Content = "Clear";
  Border^ hBorder3 = gcnew Border();
  hBorder3->Width = 120;
  hBorder3->Height = 30;
  hBorder3->BorderThickness = Thickness(1);
  hBorder3->BorderBrush = Brushes::Black;
  hBorder3->Child = hClearButton;
  TextBlock^ hSpacer = gcnew TextBlock();
  hSpacer->Width = 10;

  hStackPanel1->Children->Add(hBorder7);
  hStackPanel1->Children->Add(hSpacer);
  hStackPanel1->Children->Add(hBorder3);
  hStackPanel1->Orientation = Orientation::Horizontal;
  hStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
  hSendMessageGrid->SetRow(hStackPanel1, 1);
  hSendMessageGrid->Children->Add(hStackPanel1);

  SetButtonPropertiesHelper1();
}

void WPFCppCliDemo::SetButtonPropertiesHelper1()
{

	//Set radio buttons
	Border^ hBorder4 = gcnew Border();
	hBorder4->Width = 230;

	Border^ hBorder5 = gcnew Border();
	hBorder5->Width = 30;

	hSendMessageGrid->SetRow(radioStackPubPanel, 2);
	hSendMessageGrid->Children->Add(radioStackPubPanel);

	radioStackPubPanel->Orientation = Orientation::Horizontal;
	radioStackPubPanel->Children->Add(hBorder4);

	catpub1->Content = "Category1";
	catpub1->GroupName = "options2";
	radioStackPubPanel->Children->Add(catpub1);
	radioStackPubPanel->Children->Add(hBorder5);

	catpub2->Content = "Category2";
	catpub2->GroupName = "options2";
	radioStackPubPanel->Children->Add(catpub2);
	hBorder5 = gcnew Border();
	hBorder5->Width = 30;
	radioStackPubPanel->Children->Add(hBorder5);

	catpub3->Content = "Category3";
	catpub3->GroupName = "options2";
	radioStackPubPanel->Children->Add(catpub3);
	hBorder5 = gcnew Border();
	hBorder5->Width = 30;
	radioStackPubPanel->Children->Add(hBorder5);

	catpub1->IsChecked = "true";
}

void WPFCppCliDemo::setDownloadButtonProperties()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hDownloadGrid->RowDefinitions->Add(hRow2Def);

	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(75);
	hDownloadGrid->RowDefinitions->Add(hRow3Def);

	hDownloadCatButton->Content = "Download All Files from\n Selected Category";
	Border^ hBorder7 = gcnew Border();
	hBorder7->Width = 150;
	hBorder7->Height = 40;
	hBorder7->BorderThickness = Thickness(1);
	hBorder7->BorderBrush = Brushes::Black;
	hBorder7->Child = hDownloadCatButton;

	hDisplayCat->Content = "Display Categories";
	Border^ hBorder6 = gcnew Border();
	hBorder6->Width = 120;
	hBorder6->Height = 30;
	hBorder6->BorderThickness = Thickness(1);
	hBorder6->BorderBrush = Brushes::Black;
	hBorder6->Child = hDisplayCat;

	hdisplayCatFiles->Content = "Display All Source Code \nFiles from Selected Category";
	Border^ hBorder8 = gcnew Border();
	hBorder8->Width = 160;
	hBorder8->Height = 40;
	hBorder8->BorderThickness = Thickness(1);
	hBorder8->BorderBrush = Brushes::Black;
	hBorder8->Child = hdisplayCatFiles;

	hDownloadSelectedFilesFromList->Content = "Download Selected Html \nFiles from above list";
	Border^ hBorder9 = gcnew Border();
	hBorder9->Width = 150;
	hBorder9->Height = 40;
	hBorder9->BorderThickness = Thickness(1);
	hBorder9->BorderBrush = Brushes::Black;
	hBorder9->Child = hDownloadSelectedFilesFromList;

	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;

	setDownloadButtonPropertiesHelper1(hBorder7, hBorder6, hBorder8, hBorder9, hSpacer);
	setDownloadButtonPropertiesHelper2();
}

void WPFCppCliDemo::setDownloadButtonPropertiesHelper1(Border^ hBorder7, Border^ hBorder6, Border^ hBorder8, Border^ hBorder9, TextBlock^ hSpacer )
{
	hClearDownloadButton->Content = "Clear";
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hClearDownloadButton;
	StackPanel^ hStackPanel2 = gcnew StackPanel();
	hStackPanel2->Children->Add(hBorder7);
	hStackPanel2->Children->Add(hSpacer);
	hStackPanel2->Children->Add(hBorder3);
	hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	hStackPanel2->Children->Add(hSpacer);
	hStackPanel2->Children->Add(hBorder6);
	hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	hStackPanel2->Children->Add(hSpacer);
	hStackPanel2->Children->Add(hBorder8);
	hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	hStackPanel2->Children->Add(hSpacer);
	hStackPanel2->Children->Add(hBorder9);
	hStackPanel2->Orientation = Orientation::Horizontal;
	hStackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hDownloadGrid->SetRow(hStackPanel2, 2);
	hDownloadGrid->Children->Add(hStackPanel2);
}

void WPFCppCliDemo::setDownloadButtonPropertiesHelper2()
{
	//Set radio buttons
	Border^ hBorder4 = gcnew Border();
	hBorder4->Width = 100;
	Border^ hBorder5 = gcnew Border();
	hBorder5->Width = 30;
	hNoParentFile->Content = "Display No Parent files \n from selected category";
	hNoParentFile->Width = 150;
	hNoParentFile->Height = 40;
	StackPanel^ radioPanel = gcnew StackPanel();
	hDownloadGrid->SetRow(radioPanel, 3);
	hDownloadGrid->Children->Add(radioPanel);
	radioPanel->Orientation = Orientation::Horizontal;
	radioPanel->Children->Add(hBorder4);
	radioPanel->Children->Add(hNoParentFile);
	hBorder4 = gcnew Border();
	hBorder4->Width = 30;
	radioPanel->Children->Add(hBorder4);
	catdow1->Content = "Category1";
	catdow1->GroupName = "options3";
	radioPanel->Children->Add(catdow1);
	radioPanel->Children->Add(hBorder5);
	catdow2->Content = "Category2";
	catdow2->GroupName = "options3";
	radioPanel->Children->Add(catdow2);
	hBorder5 = gcnew Border();
	hBorder5->Width = 30;
	radioPanel->Children->Add(hBorder5);
	catdow3->Content = "Category3";
	catdow3->GroupName = "options3";
	radioPanel->Children->Add(catdow3);
	hBorder5 = gcnew Border();
	hBorder5->Width = 30;
	radioPanel->Children->Add(hBorder5);
	hDeleteFileButton->Content = "Delete Selected Html \nFiles from server repo";
	Border^ hBorder10 = gcnew Border();
	hBorder10->Width = 150;
	hBorder10->Height = 40;
	hBorder10->BorderThickness = Thickness(1);
	hBorder10->BorderBrush = Brushes::Black;
	hBorder10->Child = hDeleteFileButton;
	radioPanel->Children->Add(hBorder10);
	catdow1->IsChecked = "true";
}

void WPFCppCliDemo::StartIIS(Object^ obj, RoutedEventArgs^ args)
{
	String^ x = hIISTextBox->Text;
	std::string cat = "";

	if (catIIS1->IsChecked)
		cat = "Category1";
	else if (catIIS2->IsChecked)
		cat = "Category2";
	else
		cat = "Category3";

	std::string msg = "IIS," + cat +"," + toStdString(x);
	pSendr_->postMessage(msg);
}

void WPFCppCliDemo::DeleteSelectedFiles(Object^ obj, RoutedEventArgs^ args)
{
	String^ msg = "Delete,";
	String^ cat = "";
	if (catdow1->IsChecked)
		cat = "Category1";
	else if (catdow1->IsChecked)
		cat = "Category2";
	else
		cat = "Category3";

	msg = msg + cat + ",";
	int count = hListBox2->SelectedItems->Count;
	if (count > 0)
	{
		for each(String^ item in hListBox2->SelectedItems)
		{
			msg += item + ",";
		}
		pSendr_->postMessage(toStdString(msg));
	}
	else
	{
		pSendr_->postMessage(toStdString("No files Selected"));
	}
}

void WPFCppCliDemo::setUpLazyDownloadView()
{
	//Console::Write("\n setting up lazy download view");
	hLazyDownloadGrid->Margin = Thickness(20);
	hLazyDownloadTab->Content = hLazyDownloadGrid;

	//Set up text block for lazy download
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hLazyDownloadGrid->RowDefinitions->Add(hRow1Def);
	hRow1Def->Height = System::Windows::GridLength(200);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hTextBlock3;
	hTextBlock3->Padding = Thickness(15);
	hTextBlock3->Text = "";
	hTextBlock3->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBlock3->FontWeight = FontWeights::Bold;
	hTextBlock3->FontSize = 16;
	hScrollViewer3->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer3->HorizontalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer3->Content = hBorder1;	
	hScrollViewer3->ScrollToEnd();
	hLazyDownloadGrid->SetRow(hScrollViewer3, 0);
	hLazyDownloadGrid->Children->Add(hScrollViewer3);

	//Set up list box in lazy download view
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hLazyDownloadGrid->RowDefinitions->Add(hRow2Def);
	hRow2Def->Height = System::Windows::GridLength(150);
	Border^ hBorder2 = gcnew Border();
	hBorder2->Margin = Thickness(10);
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hListBox3;
	hLazyDownloadGrid->SetRow(hBorder2, 1);
	hLazyDownloadGrid->Children->Add(hBorder2);

	//Add Buttons
	Border^ hBorder3 = gcnew Border();
	hBorder3->Margin = Thickness(10);
	hBorder3->Height = 50;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hLazyDButton1;
	hLazyDButton1->Content = "Get files from \n Selected Category";

	setUpLazyDownloadViewHelper1(hBorder3);
	setUpLazyDownloadViewHelper2();
}

void WPFCppCliDemo::setUpLazyDownloadViewHelper1(Border^ hBorder3)
{
	Border^ hBorder4 = gcnew Border();
	hBorder4->Margin = Thickness(10);
	hBorder4->Height = 50;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hLazyDButton2;
	hLazyDButton2->Content = "Lazy Download Seleted File";

	Border^ hBorder6 = gcnew Border();
	hBorder6->Margin = Thickness(10);
	hBorder6->Height = 50;
	hBorder6->Width = 100;
	hBorder6->BorderThickness = Thickness(1);
	hBorder6->BorderBrush = Brushes::Black;
	hBorder6->Child = hLazyDButton3;
	hLazyDButton3->Content = "Clear";

	//Add Stack Panel

	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hLazyDownloadGrid->RowDefinitions->Add(hRow3Def);

	hRow3Def->Height = System::Windows::GridLength(100);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;

	hLazyStackPanel1->Children->Add(hBorder3);
	hLazyStackPanel1->Children->Add(hSpacer);
	hLazyStackPanel1->Children->Add(hBorder4);
	hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	hLazyStackPanel1->Children->Add(hSpacer);
	hLazyStackPanel1->Children->Add(hBorder6);
	hLazyStackPanel1->Orientation = Orientation::Horizontal;
	hLazyStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hLazyDownloadGrid->SetRow(hLazyStackPanel1, 2);
	hLazyDownloadGrid->Children->Add(hLazyStackPanel1);
}

void WPFCppCliDemo::setUpLazyDownloadViewHelper2()
{
	//Add Radio Buttons
	catlaz1->Content = "Category1";
	catlaz2->Content = "Category2";
	catlaz3->Content = "Category3";

	catlaz1->GroupName = "Options3";
	catlaz2->GroupName = "Options3";
	catlaz3->GroupName = "Options3";

	hLazyStackPanel2->Children->Add(catlaz1);
	Border^ hBorder5 = gcnew Border();
	hBorder5->Width = 30;
	hLazyStackPanel2->Children->Add(hBorder5);

	hLazyStackPanel2->Children->Add(catlaz2);
	hBorder5 = gcnew Border();
	hBorder5->Width = 30;
	hLazyStackPanel2->Children->Add(hBorder5);

	hLazyStackPanel2->Children->Add(catlaz3);
	hBorder5 = gcnew Border();
	hBorder5->Width = 30;
	hLazyStackPanel2->Children->Add(hBorder5);

	hLazyStackPanel2->Orientation = Orientation::Horizontal;
	hLazyStackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	RowDefinition^ hRow4Def = gcnew RowDefinition();
	hLazyDownloadGrid->RowDefinitions->Add(hRow4Def);

	hLazyDownloadGrid->SetRow(hLazyStackPanel2, 3);
	hLazyDownloadGrid->Children->Add(hLazyStackPanel2);

	catlaz1->IsChecked = "true";
}

void WPFCppCliDemo::setUpSendMessageView()
{
 // Console::Write("\n  setting up Upload view");
  hSendMessageGrid->Margin = Thickness(20);
  hSendMessageTab->Content = hSendMessageGrid;

  setTextBlockProperties();
  setButtonsProperties();
}

std::string WPFCppCliDemo::toStdString(String^ pStr)
{
  std::string dst;
  for (int i = 0; i < pStr->Length; ++i)
    dst += (char)pStr[i];
  return dst;
}

void WPFCppCliDemo::PublishFiles(Object^ obj, RoutedEventArgs^ args)
{
	String^ cat = "";
	if (catpub1->IsChecked)
		cat = "Category1";
	else if (catpub2->IsChecked)
		cat = "Category2";
	else
		cat = "Category3";
  
	std::cout << "\n\n --> Publishing all files present in Category: " << toStdString(cat) <<" (** REQUIREMENT 3 & 4)";
  pSendr_->postMessage(toStdString("Publish,"+cat));
  Console::Write("\n  sent message");
  hStatus->Text = "Sent message";
}

void WPFCppCliDemo::NoParentFile(Object^ obj, RoutedEventArgs^ args)
{
	String^ cat = "";
	if (catdow1->IsChecked)
		cat = "Category1";
	else if (catdow2->IsChecked)
		cat = "Category2";
	else
		cat = "Category3";

	std::cout << " \n\n --> Sending message to server to display files with no parents in category:  " << toStdString(cat);
	pSendr_->postMessage(toStdString("NoParent," + cat));
	Console::Write("\n  sent message");
	hStatus->Text = "Sent message";
}

void WPFCppCliDemo::DownloadFiles(Object^ obj, RoutedEventArgs^ args)
{
	String^ cat = "";
	if (catdow1->IsChecked)
		cat = "Category1";
	else if (catdow2->IsChecked)
		cat = "Category2";
	else
		cat = "Category3";

	std::cout << " \n\n --> Sending message to server to download all files present in category: " << toStdString(cat);
	pSendr_->postMessage(toStdString("Download,"+cat));
	Console::Write("\n  sent message");
	hStatus->Text = "Sent message";
}

String^ WPFCppCliDemo::toSystemString(std::string& str)
{
  StringBuilder^ pStr = gcnew StringBuilder();
  for (size_t i = 0; i < str.size(); ++i)
    pStr->Append((Char)str[i]);
  return pStr->ToString();
}

void WPFCppCliDemo::addText(String^ msg)
{
  hTextBlock1->Text += msg + "\n\n";
  hTextBlock2->Text += msg + "\n\n";
  hTextBlock3->Text += msg + "\n\n";

  hListBox->Items->Clear();
  hListBox2->Items->Clear();
  hListBox3->Items->Clear();
}
void WPFCppCliDemo::addItem(String^ msg)
{
	std::vector<std::string> f = Utilities::StringHelper::split(toStdString(msg));
	f.erase(f.begin());
	hListBox2->Items->Clear();
	for (std::string s : f)
	{
		hListBox2->Items->Add(toSystemString(s));
	}
}

void WPFCppCliDemo::addItemToLazyTab(String^ msg)
{
	std::vector<std::string> f = Utilities::StringHelper::split(toStdString(msg));
	f.erase(f.begin());
	hListBox3->Items->Clear();
	for (std::string s : f)
	{
		hListBox3->Items->Add(toSystemString(s));
	}
}

void WPFCppCliDemo::getMessage()
{
  // recvThread runs this function

  while (true)
  {
   // std::cout << "\n  receive thread calling getMessage()";
    std::string msg = pRecvr_->getMessage();
    String^ sMsg = toSystemString(msg);
    array<String^>^ args = gcnew array<String^>(1);
    args[0] = sMsg;
	
	std::vector<std::string> f=Utilities::StringHelper::split(toStdString(sMsg));

	Action<String^>^ act;
	if (f.at(0) == "gtf")
		act = gcnew Action<String^>(this, &WPFCppCliDemo::addItem);
	else if (f.at(0) == "gtflz")
		act = gcnew Action<String^>(this, &WPFCppCliDemo::addItemToLazyTab);
	else if (f.at(0) == "IISRet")
		act = gcnew Action<String^>(this, &WPFCppCliDemo::addToStatusBar);
	else
		act = gcnew Action<String^>(this, &WPFCppCliDemo::addText);
    Dispatcher->Invoke(act, args);  // must call addText on main UI thread
  }
}
void WPFCppCliDemo::addToStatusBar(String^ msg)
{
	hStatus->Text = msg;
}
void WPFCppCliDemo::DisplayCategories(Object^ obj, RoutedEventArgs^ args)
{
	pSendr_->postMessage(toStdString("Category1,Category2,Category3"));
}
void WPFCppCliDemo::clear(Object^ sender, RoutedEventArgs^ args)
{
  Console::Write("\n  cleared message text");
  hStatus->Text = "Cleared message";
  hTextBlock1->Text = "";
  hTextBlock2->Text = "";
  hTextBlock3->Text = "";
  
  hListBox->Items->Clear();
  hListBox2->Items->Clear();
  hListBox3->Items->Clear();
}

void WPFCppCliDemo::DisplayCatFiles(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Display Category files button pressed");
	String^ cat = "";
	if (catdow1->IsChecked)
		cat = "Category1";
	else if (catdow2->IsChecked)
		cat = "Category2";
	else
		cat = "Category3";

	pSendr_->postMessage(toStdString("GetFiles," + cat));	
}

void WPFCppCliDemo::DisplayLazyCatFiles(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Display Category files button pressed");
	String^ cat = "";
	if (catlaz1->IsChecked)
		cat = "Category1";
	else if (catlaz2->IsChecked)
		cat = "Category2";
	else
		cat = "Category3";

	selCat = cat;
	pSendr_->postMessage(toStdString("GetFilesLz," + cat));
}

void WPFCppCliDemo::clearDownload(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  cleared message text");
	hStatus->Text = "Cleared message";
	hTextBlock2->Text = "";
	hTextBlock3->Text = "";

	hListBox3->Items->Clear();
	hListBox->Items->Clear();
	hListBox2->Items->Clear();
}

void WPFCppCliDemo::uploadFilesButton(Object^ sender, RoutedEventArgs^ args)
{
	String^ path = hFolderBrowserDialog->SelectedPath;
	if (path == "")
	{
		hStatus->Text = "No Path Selected";
		return;
	}
	if (cat1->IsChecked)
	{
		pSendr_->postMessage(toStdString("Upload," + path+","+cat1->Content));
	}
	else if (cat2->IsChecked)
	{
		pSendr_->postMessage(toStdString("Upload," + path + "," + cat2->Content));
	}
	else
		pSendr_->postMessage(toStdString("Upload,"+path + "," + cat3->Content));
	hStatus->Text = "Files Sent to Server";
}

void WPFCppCliDemo::getItemsFromList(Object^ sender, RoutedEventArgs^ args)
{
	int index = 0;
	int count = hListBox->SelectedItems->Count;
	hStatus->Text = "Show Selected Items";
	array<System::String^>^ items = gcnew array<String^>(count);
	if (count > 0) {
		for each (String^ item in hListBox->SelectedItems)
		{
			items[index++] = item;
		}
	}

	hListBox->Items->Clear();
	if (count > 0) {
		for each (String^ item in items)
		{
			hListBox->Items->Add(item);
		}
	}
}

void WPFCppCliDemo::DownloadSelectedFilesFromList(Object^ obj, RoutedEventArgs^ args)
{
	String^ msg = "DownloadSelectedFiles,";
	String^ cat = "";
	if (catdow1->IsChecked)
		cat = "Category1";
	else if (catdow2->IsChecked)
		cat = "Category2";
	else
		cat = "Category3";

	msg = msg + cat + ",";
	int count = hListBox2->SelectedItems->Count;
	if (count > 0)
	{
		for each(String^ item in hListBox2->SelectedItems)
		{
			msg += item + ",";
		}
		pSendr_->postMessage(toStdString(msg));
	}
	else
	{
		pSendr_->postMessage(toStdString("No files Selected"));
	}
}

void WPFCppCliDemo::DownloadSelectedLazyFile(Object^ obj, RoutedEventArgs^ args)
{
	String^ msg = "DownloadLazy,"+selCat+",";
	if (hListBox3->SelectedItems->Count == 0)
		pSendr_->postMessage(toStdString("No files Selected"));
	else {
		msg += hListBox3->SelectedItem;
		pSendr_->postMessage(toStdString(msg));
	}
}

void WPFCppCliDemo::setUpFileListView()
{
  hFileListGrid->Margin = Thickness(20);
  hFileListTab->Content = hFileListGrid;
  RowDefinition^ hRow1Def = gcnew RowDefinition();
  hFileListGrid->RowDefinitions->Add(hRow1Def);
  Border^ hBorder1 = gcnew Border();
  hBorder1->BorderThickness = Thickness(1);
  hBorder1->BorderBrush = Brushes::Black;
  hListBox->SelectionMode = SelectionMode::Multiple;
  hBorder1->Child = hListBox;
  hFileListGrid->SetRow(hBorder1, 0);
  hFileListGrid->Children->Add(hBorder1);  
  RowDefinition^ hRow2Def = gcnew RowDefinition();
  hRow2Def->Height = GridLength(75);
  RowDefinition^ hRow2Def2 = gcnew RowDefinition();
  hRow2Def2->Height = GridLength(75);
  RowDefinition^ hRow2Def3 = gcnew RowDefinition();
  hRow2Def3->Height = GridLength(75);
  RowDefinition^ hRow2Def4 = gcnew RowDefinition();
  hRow2Def4->Height = GridLength(75);
  hFileListGrid->RowDefinitions->Add(hRow2Def);
  hFileListGrid->RowDefinitions->Add(hRow2Def2);
  hFileListGrid->RowDefinitions->Add(hRow2Def3);
  hFileListGrid->RowDefinitions->Add(hRow2Def4);
  hFolderBrowseButton->Content = "Select Directory";
  hFolderBrowseButton->Height = 30;
  hFolderBrowseButton->Width = 120;
  hFolderBrowseButton->BorderThickness = Thickness(2);
  hFolderBrowseButton->BorderBrush = Brushes::Black;
  hFileListGrid->SetRow(hFolderBrowseButton, 1);
  hFileListGrid->Children->Add(hFolderBrowseButton);  
  setUpFileListViewHelper1();
}

void WPFCppCliDemo::setUpFileListViewHelper1()
{
	//Upload Files Button
	hUploadFilesButton->Content = "Upload Files";
	hUploadFilesButton->Height = 30;
	hUploadFilesButton->Width = 120;
	hUploadFilesButton->BorderThickness = Thickness(2);
	hUploadFilesButton->BorderBrush = Brushes::Black;
	hFileListGrid->SetRow(hUploadFilesButton, 2);
	hFileListGrid->Children->Add(hUploadFilesButton);
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 230;
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 30;
	hFileListGrid->SetRow(radioStackPanel, 3);
	hFileListGrid->Children->Add(radioStackPanel);
	radioStackPanel->Orientation = Orientation::Horizontal;
	radioStackPanel->Children->Add(hBorder3);
	cat1->Content = "Category1";
	cat1->GroupName = "options1";
	radioStackPanel->Children->Add(cat1);
	radioStackPanel->Children->Add(hBorder2);
	cat2->Content = "Category2";
	cat2->GroupName = "options1";
	radioStackPanel->Children->Add(cat2);
	hBorder2 = gcnew Border();
	hBorder2->Width = 30;
	radioStackPanel->Children->Add(hBorder2);
	cat3->Content = "Category3";
	cat3->GroupName = "options1";
	radioStackPanel->Children->Add(cat3);
	hBorder2 = gcnew Border();
	hBorder2->Width = 30;
	radioStackPanel->Children->Add(hBorder2);
	cat1->IsChecked = "true";
	hFolderBrowserDialog->ShowNewFolderButton = false;
	TextBlock^ t = gcnew TextBlock();
	t->Text = "PLEASE READ readme.txt BEFORE PROCESSING THE GUI";
	t->FontWeight = FontWeights::Bold;
	t->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hFileListGrid->SetRow(t, 4);
	hFileListGrid->Children->Add(t);
}

void WPFCppCliDemo::browseForFolder(Object^ sender, RoutedEventArgs^ args)
{
  std::cout << "\n  Browsing for folder";
  hListBox->Items->Clear();
  System::Windows::Forms::DialogResult result;
  result = hFolderBrowserDialog->ShowDialog();
  if (result == System::Windows::Forms::DialogResult::OK)
  {
    String^ path = hFolderBrowserDialog->SelectedPath;
    std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
    array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
    for (int i = 0; i < files->Length; ++i)
      hListBox->Items->Add(files[i]);
    array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
    for (int i = 0; i < dirs->Length; ++i)
      hListBox->Items->Add(L"<> " + dirs[i]);
  }
}


void WPFCppCliDemo::setUpConnectionView()
{
  //Console::Write("\n  setting up Download view");

  hDownloadGrid->Margin = Thickness(1);
  hDownloadTab->Content = hDownloadGrid;

  setDownloadTabTextProperties();
  setDownloadButtonProperties();
}

void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
  Console::Write("\n  Window loaded");
}

void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
  Console::Write("\n  Window closing");
}

//Test Executive
[STAThread]
int main(array<System::String^>^ args)
{
  Console::WriteLine(L"\n ======================================================== ");
  Console::WriteLine(L"\n\t STARTING WPF CLIENT");
  Console::WriteLine(L"\n ======================================================== ");
  std::cout << "\n\n NOTE: The performance metrics that are exceeding on applying codeanalyzer on this project have not been implemented or modified by me!!.";


  Application^ app = gcnew Application();
  app->Run(gcnew WPFCppCliDemo());
  
  Console::WriteLine(L"\n\n");
}