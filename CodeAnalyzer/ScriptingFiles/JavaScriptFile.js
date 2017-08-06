/*
JavaScriptFile.js- JavaScript File to implement Expand/Collapse Functionality 
 Published: 4th April 2017 
Nikhil Prashar, CSE687 - Object Oriented Design, Spring 2017
*/
function myFunction(spanElement,buttonElement) {
    var x = document.getElementById(spanElement);
    if (x.style.display === 'none') {
		document.getElementById(buttonElement).innerHTML = "-"
        x.style.display = 'inline';
    } else {
		document.getElementById(buttonElement).innerHTML = "+"
        x.style.display = 'none';
    }
}