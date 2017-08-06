/////////////////////////////////////////////////////////////////////////////
// CodePublisher.cpp - support Code Publishing for a set of files           //
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

#include "CodePublisher.h"

//Constructor to initialize various paths coming from command line
CodePublisher::CodePublisher(std::string _scriptingPath, std::string _webPageRepo,
	std::string _htmlHeader, std::string _startUpPageHeader, std::vector<std::string> impP) :
	scriptingFilesPath(_scriptingPath), webPageRepoPath(_webPageRepo),
	htmlHeader(_htmlHeader), startPageHeader(_startUpPageHeader),
	impPackages(impP)
{
	startUpPageName = "startUpPage.htm";
	cTime = DateTime_ToString(time(0));
	std::replace(cTime.begin(), cTime.end(), ':', '-');
}

//This function extracts dependencies from NoSqlDB for a given file
std::vector<std::string> CodePublisher::extractDependencyFromDB(NoSqlDB<std::string>& db, const std::string& file)
{
	Element<std::string> elem = db.RetrieveValueFromDB(file);
	std::vector<std::string> deps;
	for (auto child : elem.children)
	{
		deps.push_back(child);
	}
	return deps;
}

//This function builds Dependency Tags that are embedded in the Html Pages
std::string CodePublisher::buildDependencyDisplayTags(const std::string& file, std::vector<std::string> deps)
{
	std::string depDisplayTag = "<table id=\"myTable\" style=\"border:0\"><tr><th style=\"border:0\">";
	depDisplayTag += "<h1>" + FileSystem::Path::getName(file) + "</h1></th>";
	depDisplayTag += "<th style=\"border:0\"><h3 align=\"right\"><a href=\"" + startUpPageName + "\">Back to Table Of Contents</a></h3></th></tr></table>";
	depDisplayTag += "<hr />\n";
	depDisplayTag += "<div class=\"indent\">\n";
	depDisplayTag += "<h4>dependencies:</h4>\n";

	std::cout << "\n\t--> Building Dependency Tags!!";

	for (auto dep : deps)
	{
		//For each dep found, adding an anchor tag in the HTML PAGE
		std::string name = FileSystem::Path::getName(dep);
		std::string anchor = "<a href=\"" + name + ".htm" + "\">" + name + "</a>\n";
		depDisplayTag += anchor;
		std::cout << "\n\t\t--> Adding Dependency: " << name;
	}

	if (deps.size() == 0)
	{
		depDisplayTag += " THIS FILE HAS NO DEPENDENCIES!!\n";
		std::cout << "\n\t\t--> This File has NO DEPENDENCIES!!";
	}
		
	//Ending the div for dependency tags
	depDisplayTag += "</div>\n<hr />\n";
	return depDisplayTag;
}

//Expand/Collapse functionality on mouse click
void CodePublisher::hideOnMouseClick(std::string& fileContent)
{
	std::cout << "\n\t--> Processing Expand/Collapse Functionality on mouse click for this file!!";
	CodeAnalysis::ScopeStack<char> ss;
	std::string temp = "";
	int id = 0;
	int newlineCounter = 0;
	for (size_t i = 0; i < fileContent.length(); i++)
	{
		if (fileContent[i] == '{' && fileContent[i-1]!='\'')
		{	//adding a <span> tag to its scope 
			ss.push(fileContent[i]);
			id++;
			int j = 0;

			for (j = temp.length() - 1; temp[j] == '\n' || temp[j] == '\t'; j--);

			for (; temp[j] != '\n' && temp[j] != '\t'; j--);

			std::string buttonId = "button" + std::to_string(id);
			//Adding a button to implement expand/collapse on the <span> scope
			if (temp[j] == '\t')
				temp.insert(j, "<button onClick = \"myFunction('" + std::to_string(id) + "','" + buttonId + "')\" id=\"" + buttonId + "\">-</button>");
			else
				temp.insert(j + 1, "<button onClick = \"myFunction('" + std::to_string(id) + "','" + buttonId + "')\" id=\"" + buttonId + "\">-</button>");
			temp += fileContent[i];
			temp += "<span id=\"" + std::to_string(id) + "\">";
		}
		else if (fileContent[i] == '}' && fileContent[i - 1] != '\'')
		{
			//closing the <span> tag and ending scope
			ss.pop();
			temp += "</span>";
			temp += fileContent[i];
		}
		else
			temp += fileContent[i];
	}
	//copying temp string created to original fileContent string
	fileContent = temp;
}

//Building html header anc creating the Html Pages
void CodePublisher::buildHtmlHeaderAndcopy(std::string& fileContent, const std::string& file,
	NoSqlDB<std::string>& db, std::vector<std::string> deps)
{
	//Reading common header from html header file
	FileSystem::File htmlHeader(FileSystem::Path::getFullFileSpec(scriptingFilesPath+htmlHeader));
	htmlHeader.open(htmlHeader.in);

	std::string header = htmlHeader.readAll(true);

	//building dep tags
	std::string depDisplayTag = buildDependencyDisplayTags(file, deps);

	fileContent = "<html>\n" + header + "<body>\n" + depDisplayTag + "<pre>\n" + fileContent + "</pre>\n</body>\n</html>";

	//building expan/collapse functionality
	hideOnMouseClick(fileContent);


	//Creating Html Web Page File
	std::FILE* myfile;
	FileSystem::Directory::create(webPageRepoPath + cTime);
	std::string filePath = webPageRepoPath + cTime + "/" + FileSystem::Path::getName(file) + ".htm";
	fileContent = "<!--\n" + FileSystem::Path::getName(file) + ".htm" +
		"- Html file for " + FileSystem::Path::getName(file) +
		"\n Published: 4th April 2017 " +
		"\nNikhil Prashar, CSE687 - Object Oriented Design, Spring 2017\n-->\n" + fileContent;

	fopen_s(&myfile, filePath.c_str(), "w+");
	if (myfile == NULL)
		throw "Invalid Directory Path!!";
	fprintf(myfile, fileContent.c_str());
	fclose(myfile);
	std::cout << "\n\t--> Publishing finished!! File: " << filePath << " has been published!!";	
}

//Copy file content to a string to start processing
void CodePublisher::copytoHtmlFiles(const std::vector<std::string>& files, NoSqlDB<std::string>& db)
{
	createHtmlCommonHeaders();

	for (auto file : files)
	{	
		//for every file, copying data to a string
		std::cout << "\n\n--> Code Publishing Started for file: " << file;
		FileSystem::File f(FileSystem::Path::getFullFileSpec(file));
		f.open(f.in);
		std::string fileContent = f.readAll(true);
		int i = 0;

		//Replacing < with &lt;
		for (;;) {
			i = fileContent.find("<", i);
			if (i == std::string::npos) {
				break;
			}
			fileContent.replace(i, 1, "&lt;");
		}

		//replacing > with &gt;
		i = 0;
		for (;;) {
			i = fileContent.find(">", i);
			if (i == std::string::npos) {
				break;
			}
			fileContent.replace(i, 1, "&gt;");
		}

		std::cout << "\n\t -->Extracting Dependencies from Database!!";
		std::vector<std::string> dependencies = extractDependencyFromDB(db, file);

		buildHtmlHeaderAndcopy(fileContent, file, db, dependencies);
	}

}

//Building the start up web page file
void CodePublisher::buildStartUpPage(const std::string& rows,std::vector<std::string> a)
{
	FileSystem::File startUpPageHeader(FileSystem::Path::getFullFileSpec(scriptingFilesPath+startPageHeader));
	startUpPageHeader.open(startUpPageHeader.in);

	std::string header = startUpPageHeader.readAll(true);

	int index = header.find("</table>");
	header.insert(index, rows);

	//Creating startup webpage file for the code publisher 
	std::FILE* myfile;
	std::string filePath = webPageRepoPath + cTime + "/" + startUpPageName;
	header = "<!--\nStartUpPage.htm- Start up web page for the Code Publisher "
		"\n Published: 4th April 2017 " 
		"\nNikhil Prashar, CSE687 - Object Oriented Design, Spring 2017\n-->\n" + header;

	fopen_s(&myfile, filePath.c_str(), "w+");
	if (myfile == NULL)
		throw "Invalid Directory Path!!";
	fprintf(myfile, header.c_str());
	fclose(myfile);	
}

//Opening the startup webpage
void CodePublisher::openStartUpPage()
{
	std::cout << "\n\n--> Opening Startup Web Page...";
	
	std::string d = "";
	for (size_t i = 0; i < webPageRepoPath.length(); i++)
	{
		if (webPageRepoPath[i] == '/')
			d += "\\";
		else
			d += webPageRepoPath[i];
	}
	d += cTime+"\\"+startUpPageName;

	//using Shell Execute to open the Startup webpage file
	std::wstring wideURL;
	for (size_t i = 0; i < d.length(); ++i)
		wideURL += wchar_t(d[i]);

	const wchar_t* url = wideURL.c_str();
	ShellExecute(NULL, NULL, url, NULL, NULL, SW_SHOWNORMAL);	
}

//Creating Common html header for all htm files
void CodePublisher::createHtmlCommonHeaders()
{
	//build html Common Header
	std::vector<std::string> cssFile = FileSystem::Directory::getFiles(scriptingFilesPath, "*.css");
	std::vector<std::string> jsFile = FileSystem::Directory::getFiles(scriptingFilesPath, "*.js");
	std::string htmHeaderContent = "<head>\n<link rel = \"stylesheet\" href = \"../" +
		scriptingFilesPath + cssFile.at(0) + "\" />\n";
	
	htmHeaderContent += "<script src=\"../" + scriptingFilesPath + jsFile.at(0) + "\"></script>\n</head>\n";

	//Creating file for common html header
	std::FILE* myfile;
	std::string filePath = scriptingFilesPath + htmlHeader;
	filePath = FileSystem::Path::getFullFileSpec(filePath);
	fopen_s(&myfile, filePath.c_str(), "w+");
	if (myfile == NULL)
		throw "Invalid Directory Path!!";
	std::string tempH = htmHeaderContent;
	htmHeaderContent = "<!--\n" + htmlHeader + "- Common html header file."
		"\n Published: 4th April 2017 "
		"\nNikhil Prashar, CSE687 - Object Oriented Design, Spring 2017\n-->\n" + htmHeaderContent;

	fprintf(myfile, htmHeaderContent.c_str());
	fclose(myfile);
	
	//Build StartUp Page Header
	std::string startPageHeaderContent = "<html>\n" + tempH;
	startPageHeaderContent += "\n<body><h1 align = \"center\">WELCOME TO CODE PUBLISHER</h1>";
	startPageHeaderContent += "<hr /><br /><h2 align = \"center\">TABLE OF CONTENT</h2><p align=\"center\">Note: Important Packages-";
	for (std::string imp : impPackages)
		startPageHeaderContent += imp + ", ";
	startPageHeaderContent += "</p>";
	startPageHeaderContent += "\n<table width = \"70%\" align = \"center\">";
	startPageHeaderContent+="<tr>\n<th>Files</th>\n<th>Path</th>\n</tr></table></body></html>";

	//Creating startup page common header file
	std::FILE* myfile2;
	filePath = scriptingFilesPath + startPageHeader;
	fopen_s(&myfile2, filePath.c_str(), "w+");
	if (myfile == NULL)
		throw "Invalid Directory Path!!";
	startPageHeaderContent = "<!--\n" + startPageHeader + "- Start Page header file."
		"\n Published: 4th April 2017 "
		"\nNikhil Prashar, CSE687 - Object Oriented Design, Spring 2017\n-->\n" + startPageHeaderContent;

	fprintf(myfile2, startPageHeaderContent.c_str());
	fclose(myfile2);	
}

#ifdef TEST_CODEPUBLISHER
int main(int argc,char* argv[])
{
	CodeAnalysis::CodeAnalysisExecutive exec;
	exec.copyImportantProjectFiles(argv);
	exec.ExtractFromCommandLine(argc, argv);
	exec.InitialAnalysis(argc, argv, exec);
	exec.FileAnalysis();
}
#endif