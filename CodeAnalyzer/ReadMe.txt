readme.txt for Project 4
----------------------------

In this file , I am only going to talk about the different Repositories for the project and how to process the GUI. 

Repositories
--------------
RemoteServerRepository - This folder will contain the source code files that the client uploads to the server (Based on the Category).

RemoteWebPageRepository - This folder will contain the Published HTML Files that the client publishes from the 
							available source code files in RemoteServerRepository folder. (Based on the Category).

LocalClientRepository - This is the Clients local repository where the downloaded files get stored (Based on Category). 


HOW TO PROCESS THE GUI FOR BEST RESULTS
-----------------------------------------

1) UPLOAD TAB
--------------
--> In the upload tab, you can select in which category you want to upload the source code files.
	Click on select directory, select the folder you want to upload and click the upload button to upload the source code files. 
	The files will get stored in RemoteServerRepository/name_of_the_category_you_specified.
	
2) PUBLISH TAB
--------------
--> In the publish tab, please select which category of files you want to publish (the category you uploaded the files to in the upload tab)
	and then click the publish button to publish the files present in the specified category. The files will get saved in 
	RemoteWebPageRepository/name_of_the_category_you_specified

3) DOWNLOAD/DELETE TAB
-------------------------
--> You can select any category and play around with the different buttons. Basically this tab will download files to the LocalClientRepository folder 
	based on which category you select. You can also select files from the list box and download/delete only the selected files. 
	NOTE: In the list box, i am displaying the source code files present in the server repository, but when you delete or download after selecting
			those files, the html files will get deleted/downloaded (this is how i have implemented my design). 
	This tab satisfies all the requirements specified in the Purpose section.
	
4) Lazy Download Tab
-------------------------
--> Select a category and click on get files. This will retrieve all the files present in the server repository for the category specified. 
	Then , select any file from the listbox and click on lazy download. All files linked to it (lazy algo) will get saved in 
	LocalClientRepository/LazyDownload-Timestamp folder. 

5) IIS Tab
----------------
--> Please copy and paste the Absolute path into the textbox, and select a category and then click on start IIS. 
	NOTE: the path has to be Absolute : example - S:\OOD\OOD\Project4\CodeAnalyzer\IISFolder
	IISFolder is set up as the IIS path. 
	IMPORTANT NOTE*** - to view the IIS in browser, please type localhost:8080/name_of_the_category_you_specified. 
	If you type just localhost:8080 in the browser URL, a default page would pop up (specified by me), stating what to mention in the URL. 
	




