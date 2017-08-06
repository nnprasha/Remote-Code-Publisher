#pragma once
//////////////////////////////////////////////////////////////////////////////
// HttpMessage.h -Build and parse Http style messages						//
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
* PUBLIC INTERFACE: 
* -----------------
*  void addAttribute(const Attribute& attrib) - Adds an attribute to the message
*  Value findValue(const Name& name) - finds value of an attribute
*  size_t findAttribute(const Name& name) - finds the attribute and returns the index
*  bool removeAttribute(const Name& name) - removes attribute
*  static std::string attribString(const Attribute& attrib) - returns string rep of attribute
*  static Attribute attribute(const Name& name, const Value& value) - returns attribute
*  static Attribute parseAttribute(const std::string& src) - parses all attributes
*
* 
*  void setBody(byte buffer[], size_t Buflen) - sets the body of the message type
*  size_t getBody(byte buffer[], size_t& Buflen) - returns index of body
*  void addBody(const Body& body) - add body element
*  void addBody(const std::string& body) - adds body element
*  void addBody(size_t numBytes, byte* pBuffer) - adds body element
*  size_t bodyLength() - returns length of message body
*
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
#include <vector>
#include <string>
#include <array>

class HttpMessage
{
public:
  using byte = char;
  using Name = std::string;
  using Value = std::string;
  using Attribute = std::pair<Name, Value>;
  using Attributes = std::vector<Attribute>;
  using Terminator = std::string;
  using Body = std::vector<byte>;

  // message attributes
  void addAttribute(const Attribute& attrib);
  Value findValue(const Name& name);
  size_t findAttribute(const Name& name);
  bool removeAttribute(const Name& name);
  Attributes& attributes();
  static std::string attribString(const Attribute& attrib);
  static Attribute attribute(const Name& name, const Value& value);
  static Attribute parseAttribute(const std::string& src);

  // message body
  void setBody(byte buffer[], size_t Buflen);
  size_t getBody(byte buffer[], size_t& Buflen);
  void addBody(const Body& body);
  void addBody(const std::string& body);
  void addBody(size_t numBytes, byte* pBuffer);
  Body& body();
  size_t bodyLength();

  // display
  std::string headerString() const;
  std::string bodyString() const;
  std::string toString() const;
  std::string toIndentedHeaderString() const;
  std::string toIndentedBodyString() const;
  std::string toIndentedString() const;

  // cleanup
  void clear();
  static void fillBuffer(byte buffer[], size_t BufSize, byte fill = '\0');

private:
  Attributes attributes_;
  Terminator term_ = "\n";
  Body body_;
};