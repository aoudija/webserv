#ifndef REQEST_HPP
#define REQEST_HPP

#include <iostream>

#include <filesystem>

#include <string>
#include <sstream>
#include <map>
#include <dirent.h>

#include <vector>
#include "../server.hpp"
#include "../Parsing/Location.hpp"

using std::string;
using std::map;
using std::vector;
class server;
class request
{
	std::string method;
	std::string requestURI;
	std::string httpVersion;
	std::string body;

	map<std::string, std::string> allContTypes;
	string ContentType;
	string uploadContentType;
	int bytesRange;

	int chunkSize;
	std::string filePath;
	std::string redirectURL;

	int bodyContentLength;

	bool requestStatus;

	std::string statusCode;

	std::string	queryString;
	// std::string kolchi;
	std::string	boundary;


	int flag;
	int gg;
	std::string filename;

	//ayoub
	string cgi_header;
	string cgi_body;
public:
	int bodyDone;
	int headersDone;
	bool failHeader;
	std::string headers;//before parsing
	std::string theBody;//after parsing

	std::map<std::string, std::string> headerFields;
	Location loc;
	int actualContentLength;
	enum ParsingStatus {
	ParsingDone,
	ParsingFailed,
	ParsingContinue,
	};

	enum ChunkedBodyState {
		Initial,
		ParsingChunkSize,
		AfterChunkSizeSpace,
		ChunkSizeCarriageReturn,
		ParsingChunkData,
		ChunkDataCarriageReturn,
		ChunkDataCrLf,
	};
	request();
	request(std::string req, server _server);
	request(const request &other);
	request& operator=(const request& other);
	// ~request();

	int getBodyRequest(std::string requestPart);
	int getHeadersRequest(std::string requestPart);

	void setBytesRange();
	void setContentLength();
	int getBytesRange();
	std::string getMethod();
	std::string getHttpVersion();
	string getrequestURI();
	string getredirectURL();
	string getContentType();
	string getFilePath();
	void setFilePath(std::string filePath);
	std::string getStatusCode();
	string getQueryString();

	void setStatusCode(std::string statusCode);

	void    setContentType();
	void    setContentType(std::string contentType);
	void    addAllContentTypes();

	int checkRequestLine(std::string request);
	int checkHeaderFields(std::string headerFiles);
	int parseRequest(std::string request, server& _server);
	request::ParsingStatus checkBody(std::string body, server& _server);
	request::ParsingStatus checkBody2(std::string body, server& _server);
	request::ParsingStatus checkBody3(std::string body, server& _server);

	int matchLocation(server& _server);
	std::string removeAndSetQueryString(const std::string& uri);
	bool    isRequestDone();

	ChunkedBodyState currentChunkedState;
	
	std::string error;//unused 


	ParsingStatus parsChunked(char c, std::string fileName);
//  ayoub
	bool    is_CGI;
	std::pair<string, string> cgi_exe;
	void    setCgiHeader(std::string);
	void    setCgiBody(std::string);

	string    getCgiHeader(void);
	string    getCgiBody(void);
	//amine
	void    setmethod(string s){
		method = s;
	}
	//endamine
};

bool fileExists(const char* path);
bool isDirectory(const char* path);

std::string errorPageTamplate(std::string errorMessage);

// void parseRequest(std::string request);

#endif