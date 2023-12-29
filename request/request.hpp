#ifndef REQEST_HPP
#define REQEST_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>

using std::string;
using std::map;
using std::vector;

class request
{
	string method;
	string requestURI;
	string httpVersion;
	map<std::string, std::string> headerFields;
	string body;

public:
	request();
	request(std::string req);
	request(const request &other);
	request& operator=(const request& other);
	string getMethod();
	void checkRequestLine(std::string request);
	void checkHeaderFields(std::string headerFiles);
	void parseRequest(std::string request);
	void checkBody(std::string body);
	string getrequestURI();
	string getContentType();
};

// void parseRequest(std::string request);

#endif