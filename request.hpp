#ifndef REQEST_HPP
#define REQEST_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>

class request
{
    std::string method;
    std::string requestURI;
    std::string httpVersion;
    std::map<std::string, std::string> headerFields;
    std::string body;

public:
    request();
    request(std::string req);
    request(const request &other);
    request& operator=(const request& other);
    // ~request();

    std::string getMethod();
    void checkRequestLine(std::string request);
    void checkHeaderFields(std::string headerFiles);
    void parseRequest(std::string request);
    void checkBody(std::string body);
};

// void parseRequest(std::string request);

#endif















