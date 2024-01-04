#ifndef REQEST_HPP
#define REQEST_HPP

#include <iostream>

#include <filesystem>

#include <string>
#include <sstream>
#include <map>

#include <vector>
#include "../server.hpp"
#include "../aubb/Location.hpp"

using std::string;
using std::map;
using std::vector;
class server;
class request
{
    // int socket_listenner;
    // std::string  port;
    // std::string server_name;
    
    std::string method;
    std::string requestURI;
    std::string httpVersion;
    std::map<std::string, std::string> headerFields;
    std::string body;

    map<std::string, std::string> allContTypes;
    string ContentType;
    int bytesRange;

    // bodyParser b;
    // int chunkSize;
    size_t chunkSize;
    std::string filePath;

public:
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

    void setBytesRange();
    int getBytesRange();
    std::string getMethod();
    std::string getHttpVersion();
    string getrequestURI();
	string getContentType();
	string getFilePath();

    void    setContentType();
    void    addAllContentTypes();

    void checkRequestLine(std::string request);
    void checkHeaderFields(std::string headerFiles);
    void parseRequest(std::string request, server& _server);
    request::ParsingStatus checkBody(std::string body, server& _server);

    int matchLocation(server& _server);

    ChunkedBodyState currentChunkedState;
    
    std::string error;//unused 


    ParsingStatus parsChunked(char c);
};

// void parseRequest(std::string request);

#endif