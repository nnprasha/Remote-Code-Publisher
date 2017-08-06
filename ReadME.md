A Code Repository is a Program responsible for managing source code resources, e.g., files and documents. A fully developed Repository will support file persistance, managment of versions, and the acquisition and publication of source and document files. This project focuses on just the publishing functionality of a repository.

In this project we develop means to display source code files as web pages with embedded child links. Each link refers to a code file that the displayed code file depends on.

Important things to note: 

Each file to be published is a C++ source file. Our publisher will generate, for each of these, an HTML file, with most of the contents drawn from the code file.

The pages we will generate have only static content, with the exception of some embedded JavaScript and styling, so we won't need a web server.

We will need to preserve the white space structure of the displayed source code. That can be done embedding all the code between the tags <pre> and </pre> or by using the CSS white-space property with value "pre" to style a div with all the code in its contents.

Any markup characters in the code text will have to be escaped, e.g., replace < with &lt; and > with &gt;.

File dependencies are displayed in the web page with embedded links, which are implemented in HTML5 with anchor elements:
<a href="[url of referenced html page]">source code file name</a>

For each class, we will, optionally, implement outlining, similar to the visual studio outlining feature. To do that we will use the CSS display property, with values: normal or none, to control whether the contents of a div are visible or not.