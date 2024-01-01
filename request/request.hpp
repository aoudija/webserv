#ifndef REQEST_HPP
#define REQEST_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include "../server.hpp"

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

    // bodyParser b;
    // int chunkSize;
    size_t chunkSize;

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

    std::string getMethod();
    // std::string getURI();
    std::string getHttpVersion();
    string getrequestURI();
	string getContentType();

    void    setContentType();
    void    addAllContentTypes();

    void checkRequestLine(std::string request);
    void checkHeaderFields(std::string headerFiles);
    void parseRequest(std::string request, server& _server);
    request::ParsingStatus checkBody(std::string body, server& _server);

    ChunkedBodyState currentChunkedState;
    
    std::string error;

    // Typedef for parser function pointers
    typedef ParsingStatus (*ParserFunction)(request&, char);

    // Parser function declarations for chunked body states
    static ParsingStatus handleChunkStart(request&, char);
    static ParsingStatus handleChunkSize(request&, char);
    static ParsingStatus handleAfterChunkSizeSpace(request&, char);
    static ParsingStatus handleChunkSizeCarriageReturn(request&, char);
    static ParsingStatus handleChunkData(request&, char);
    static ParsingStatus handleChunkDataCarriageReturn(request&, char);
    static ParsingStatus handleChunkDataCrLf(request&, char);

    // Map of chunked body state handlers
    static ParserFunction chunkedStateHandlers[];

    // Initialize the map of chunked body state handlers
    static std::map<ChunkedBodyState, ParserFunction> initializeChunkedStateHandlers() {
        std::map<ChunkedBodyState, ParserFunction> handlers;
        handlers[Initial] = &handleChunkStart;
        handlers[ParsingChunkSize] = &handleChunkSize;
        handlers[AfterChunkSizeSpace] = &handleAfterChunkSizeSpace;
        handlers[ChunkSizeCarriageReturn] = &handleChunkSizeCarriageReturn;
        handlers[ParsingChunkData] = &handleChunkData;
        handlers[ChunkDataCarriageReturn] = &handleChunkDataCarriageReturn;
        handlers[ChunkDataCrLf] = &handleChunkDataCrLf;
        return handlers;
    }

    // Method to parse a body string
    public:
    ParsingStatus parseBody(const std::string& body);

    ParsingStatus parsChunked(char c);
};

// void parseRequest(std::string request);

#endif