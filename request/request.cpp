#include "../server.hpp"

request::request() {

}

request::request(std::string request, server _server)
{
    parseRequest(request, _server);
}

request::request(const request &other)
{
    *this = other;
}

request& request::operator=(const request& other)
{
    this->method = other.method;
    this->requestURI = other.requestURI;
    return *this;
}

void printError(std::string errorMsg, int status)
{
    std::cerr << RED << errorMsg << RESET_TEXT << std::endl;
    exit(status);
}

std::string request::getMethod() {
    return this->method;
}

string request::getrequestURI(){
    return requestURI;
}

string request::getContentType(){
    return this->ContentType;
}

std::string request::getHttpVersion() {
    return this->httpVersion;
}

void request::checkRequestLine(std::string request)
{
    std::istringstream stream(request);
    std::string line;
    std::getline(stream, line);

    std::istringstream stream2(line);

    std::cout << line << std::endl;

    stream2 >> this->method >> this->requestURI >> this->httpVersion;
    if (this->method != "GET" && this->method != "POST" && this->method != "DELETE")
        printError("Method Not Allowed", 405);
    if (this->requestURI.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%") != std::string::npos)
        printError("Bad Request", 400);
    if (this->requestURI.size() > 2048)// mazal request body larger than lbody li fl config file !!
        printError("Request-URI Too Long", 414);
    if (this->httpVersion != "HTTP/1.1")
        printError("HTTP Version Not Supported", 505);
}

void request::checkHeaderFields(std::string headerFiles)
{
    std::cout << request::method << std::endl;

    std::string line;
    std::vector<std::string> lines;
    
    std::istringstream iss(headerFiles);

    while (std::getline(iss, line)) {
        lines.push_back(line);
    }

    for (std::vector<std::string>::iterator i = lines.begin(); i != lines.end(); i++) {
        if (i->find(":") != std::string::npos) {
            this->headerFields[i->substr(0, i->find(":"))] = i->substr(i->find(":") + 1);
        }
    }
    if (headerFields.find("Transfer-Encoding") != headerFields.end()
        && headerFields["Transfer-Encoding"].find("chunked") == std::string::npos)
        printError("Not implemented", 501);
    if (headerFields.find("Transfer-Encoding") == headerFields.end()
        && headerFields.find("Content-Length") == headerFields.end()
        && this->method == "POST")
        printError("Bad Request", 400);
}

void printChar(char c)
{
    if (c == '\n')
        std::cout << CYAN << "/n" << RESET_TEXT << std::endl;
    else if (c == '\r')
        std::cout << CYAN << "/r" << RESET_TEXT << std::endl;
    else
        std::cout << CYAN << c << RESET_TEXT << std::endl;
}

request::ParsingStatus request::parsChunked(char c)
{
    switch (currentChunkedState) {
    case Initial: {
        if (c == '\r') {
            currentChunkedState = request::ChunkDataCrLf;
        }
        else if (isxdigit(c)) {
            currentChunkedState = request::ParsingChunkSize;
        } 
        else {
            // error = "jdskjfkldjs";
            return request::ParsingFailed;
        }
    }
    case ParsingChunkSize: {
        if (isxdigit(c)) {
            chunkSize = (chunkSize * 0xF + c - '0');
            break;
        }

    }
    case AfterChunkSizeSpace: {
        if (c == ' ' || c == '\t')
            break;
        if (c == '\r') {
            currentChunkedState = request::ChunkSizeCarriageReturn;
            break;
        } else if (c == ';')
        {
            // parser.error = "faild chunked body";
            return request::ParsingFailed; // specific error should be returned
        }
        else
            return request::ParsingFailed;
    }
    case ChunkSizeCarriageReturn: {
        if (c == '\n') {
            currentChunkedState = request::ParsingChunkData;
            break;
        } else
        {
            // parser.error = "faild chunked body";
            return request::ParsingFailed;
        }
    }
    case ParsingChunkData: {
        if (chunkSize == 0) {
            return request::ParsingDone;
        }
        chunkSize--;
        std::ofstream outputFile("output", std::ios::app);

        if (outputFile.is_open() && this->method != "GET")
        {
            outputFile << c;
            outputFile.close();
        }
        else
        {
            // parser.error = "faild chunked body";
            return ParsingFailed;
        }
        if (chunkSize == 0) {
            currentChunkedState = request::ChunkDataCarriageReturn;
        }
        break;
    }
    case ChunkDataCarriageReturn: {
        if (c != '\r')
        {
            // parser.error = "faild chunked body";
            return request::ParsingFailed;
        }
        currentChunkedState = request::ChunkDataCrLf;
        break;
    }
    case ChunkDataCrLf: {
        if (c != '\n')
        {
            // parser.error = "faild chunked body";
            return request::ParsingFailed;
        }
        currentChunkedState = request::Initial;
        break;
    }
}
return request::ParsingContinue;
}

request::ParsingStatus request::checkBody(std::string body, server& _server)
{
    if ((int)body.size() > _server.getClientBodyLimit())
        printError("Request Entity Too Large", 413);

    for (size_t i = 0; i < body.size(); ++i)
    {
        char currentChar = body[i];
        ParsingStatus status = parsChunked(currentChar);
        switch (status)
        {
        case ParsingContinue:
            break;// Continue parsing
        case ParsingFailed:
            return ParsingFailed;
        case ParsingDone:
            currentChunkedState = Initial;// Reset state to Initial for the next chunk
            break;
        }
    }
    return ParsingDone;
}

int request::getBytesRange()
{
    return bytesRange;
}

void request::setBytesRange()
{
    std::string input = this->headerFields["Range"];
    std::string result;

    for (std::string::iterator it = input.begin(); it != input.end(); ++it) {
        if (std::isdigit(*it)) {
            result += *it;
        }
    }
    std::istringstream(result) >> this->bytesRange;
    // std::cout << "bytes tedtts  ; " << this->headerFields["Range"] << std::endl;
}

void request::parseRequest(std::string request, server& _server)
{
    // std::cout << WHITE << request << RESET_TEXT << std::endl;
    // std::cout << RED << request << RESET_TEXT << std::endl;
    checkRequestLine(request);
    // std::cout << request.substr(0, request.find("\r\n\r\n")) << std::endl;
    
    checkHeaderFields(request.substr(0, request.find("\r\n\r\n")));

    chunkSize = 0;
    checkBody(request.substr(request.find("\r\n\r\n") + 4), _server);

    setContentType();

    std::vector<Location> vec;
    vec = _server.getLocations();
    // std::string uri = this->requestURI;
    // vector<std::string> uris;

    // while (!uri.substr(uri.rfind("/")).empty() && uri.rfind("/") == std::string::npos)
    // {
    //     uris.push_back(uri.substr(uri.rfind("/")));
    // }
    
    

    // for (size_t i = 0; i < count; i++)
    // {
    //     for (std::vector<Location>::iterator i = vec.begin(); i != vec.end(); i++) {
    //         if (requestURI == i->getPath())
    //         {
                
    //         }
        
        
    //         std::cout << MAGENTA << "URI: " << requestURI << RESET_TEXT << std::endl;
    //         std::cout << MAGENTA << "location name: " << i->getLocationName() << RESET_TEXT << std::endl;
    //         std::cout << MAGENTA << "path: " << i->getPath() << RESET_TEXT << std::endl;
    //         std::cout << MAGENTA << "root: " << i->getRoot() << RESET_TEXT << std::endl;

    
    //     }
    // }
    
}

bool isDirectory(const char* path)
{
    struct stat fileInfo;

    if (stat(path, &fileInfo) != 0) {
        return false;
    }
    return S_ISDIR(fileInfo.st_mode);// need to check if this is allowed
}

void request::setContentType()
{
    addAllContentTypes();
    if (isDirectory(requestURI.c_str())) {
        this->ContentType = "text/html";
    }
    else {
        std::string fileExtension;
        size_t dotPosition = requestURI.rfind(".");

        if (dotPosition != std::string::npos) {
            fileExtension = requestURI.substr(dotPosition);
        }
        else {
            std::cerr << "Error: No dot found in requestURI\n";
        }
        this->ContentType = allContTypes[fileExtension];
    }
}

void    request::addAllContentTypes()
{
    allContTypes[".aac"] = "audio/aac";
    allContTypes[".abw"] = "application/x-abiword";
    allContTypes[".arc"] = "application/x-freearc";
    allContTypes[".avif"] = "image/avif";
    allContTypes[".avi"] = "video/x-msvideo";
    allContTypes[".azw"] = "application/vnd.amazon.ebook";
    allContTypes[".bin"] = "application/octet-stream";
    allContTypes[".bmp"] = "image/bmp";
    allContTypes[".bz"] = "application/x-bzip";
    allContTypes[".bz2"] = "application/x-bzip2";
    allContTypes[".cda"] = "application/x-cdf";
    allContTypes[".csh"] = "application/x-csh";
    allContTypes[".css"] = "text/css";
    allContTypes[".csv"] = "text/csv";
    allContTypes[".doc"] = "application/msword";
    allContTypes[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    allContTypes[".eot"] = "application/vnd.ms-fontobject";
    allContTypes[".epub"] = "application/epub+zip";
    allContTypes[".gz"] = "application/gzip";
    allContTypes[".gif"] = "image/gif";
    allContTypes[".htm"] = "text/html";
    allContTypes[".html"] = "text/html";
    allContTypes[".ico"] = "image/vnd.microsoft.icon";
    allContTypes[".ics"] = "text/calendar";
    allContTypes[".jar"] = "application/java-archive";
    allContTypes[".jpeg"] = "image/jpeg";
    allContTypes[".jpg"] = "image/jpeg";
    allContTypes[".js"] = "text/javascript";
    allContTypes[".json"] = "application/json";
    allContTypes[".jsonld"] = "application/ld+json";
    allContTypes[".mid"] = "audio/midi";
    allContTypes[".midi"] = "audio/midi";
    allContTypes[".mjs"] = "text/javascript";
    allContTypes[".mp3"] = "audio/mpeg";
    allContTypes[".mp4"] = "video/mp4";
    allContTypes[".mpeg"] = "video/mpeg";
    allContTypes[".mpkg"] = "application/vnd.apple.installer+xml";
    allContTypes[".odp"] = "application/vnd.oasis.opendocument.presentation";
    allContTypes[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
    allContTypes[".odt"] = "application/vnd.oasis.opendocument.text";
    allContTypes[".oga"] = "audio/ogg";
    allContTypes[".ogv"] = "video/ogg";
    allContTypes[".ogx"] = "application/ogg";
    allContTypes[".opus"] = "audio/opus";
    allContTypes[".otf"] = "font/otf";
    allContTypes[".png"] = "image/png";
    allContTypes[".pdf"] = "application/pdf";
    allContTypes[".php"] = "application/x-httpd-php";
    allContTypes[".ppt"] = "application/vnd.ms-powerpoint";
    allContTypes[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    allContTypes[".rar"] = "application/vnd.rar";
    allContTypes[".rtf"] = "application/rtf";
    allContTypes[".sh"] = "application/x-sh";
    allContTypes[".svg"] = "image/svg+xml";
    allContTypes[".tar"] = "application/x-tar";
    allContTypes[".tif"] = "image/tiff";
    allContTypes[".tiff"] = "image/tiff";
    allContTypes[".ts"] = "video/mp2t";
    allContTypes[".ttf"] = "font/ttf";
    allContTypes[".txt"] = "text/plain";
    allContTypes[".vsd"] = "application/vnd.visio";
    allContTypes[".wav"] = "audio/wav";
    allContTypes[".weba"] = "audio/webm";
    allContTypes[".webm"] = "video/webm";
    allContTypes[".webp"] = "image/webp";
    allContTypes[".woff"] = "font/woff";
    allContTypes[".woff2"] = "font/woff2";
    allContTypes[".xhtml"] = "application/xhtml+xml";
    allContTypes[".xls"] = "application/vnd.ms-excel";
    allContTypes[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    allContTypes[".xml"] = "application/xml";
    allContTypes[".xul"] = "application/vnd.mozilla.xul+xml";
    allContTypes[".zip"] = "application/zip";
    allContTypes[".3gp"] = "video/3gpp"; // audio/3gpp
    allContTypes[".3g2"] = "video/3gpp2"; // audio/3gpp2
    allContTypes[".7z"] = "application/x-7z-compressed";
}