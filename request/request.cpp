#include "../server.hpp"

// std::string request::requestURI;
// std::string request::httpVersion;
// std::string request::method;
// std::map<std::string, std::string> request::headerFields;

request::request()
{

}

request::request(std::string request)
{
    parseRequest(request);
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
    std::cerr << errorMsg << std::endl;
    exit(status);
}

std::string request::getMethod() {
    return this->method;
}

// std::string request::getURI() {
//     return this->requestURI;
// }

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
    // request req;
    // std::string method, requestURI, httpVersion;

    stream2 >> this->method >> this->requestURI >> this->httpVersion;
    if (this->method != "GET" && this->method != "POST" && this->method != "DELETE")
        printError("Method Not Allowed", 405);
    if (this->requestURI.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghiklmnopqrstuvwxyz0123456789-_~：/2#0@1$80*+=") != std::string::npos)
        printError("Bad Request", 400);
    if (this->requestURI.size() > 2048)// mazal request body larger than lbody li fl config file !!
        printError("Request-URI Too Long", 414);
    if (this->httpVersion != "HTTP/1.1")
        printError("HTTP Version Not Supported", 505);
}

void request::checkHeaderFields(std::string headerFiles)
{
    // std::cout << request::method << std::endl;
    if (this->method == "POST" && (headerFiles.find("Transfer-Encoding:") == std::string::npos
        || headerFiles.find("Content-Length:") == std::string::npos))// hadi sus
        printError("Error", 21);

    std::string line;
    std::vector<std::string> lines;
    
    std::istringstream iss(headerFiles);
    std::cout << " \\\\\\ " << std::endl;
    while (std::getline(iss, line))
    {
        lines.push_back(line);
        // if (line.find(":") != std::string::npos)
        //     std::cout << line << std::endl;
    }

    for (std::vector<std::string>::iterator i = lines.begin(); i != lines.end(); i++)
    {
        if (i->find(":") != std::string::npos)
        {
            // std::string key = i->substr(0, i->find(":"));

            // std::string value = i->substr(i->find(":") + 1);
            this->headerFields[i->substr(0, i->find(":"))] = i->substr(i->find(":") + 1);

        }
    }
}

// void request::checkBody(std::string body)
// {
//     std::stringstream b(body);
//     int i = 0;
//     while (std::getline(b, body))
//     {    
//         if (isalnum(body))
//         {
//             this->body += body;
//         }
//         i++;
//         std::cout << i << "       " << body << std::endl;
//         // std::cout << "\033[1;33m" << body << CLOCK_REALTIME << std::endl;
//     }
//     std::cout << "\033[1;33m" << this->body << CLOCK_REALTIME << std::endl;
// }


// Initialize the static map of chunked body state handlers
request::ParserFunction request::chunkedStateHandlers[] = {
    &handleChunkStart,
    &handleChunkSize,
    &handleAfterChunkSizeSpace,
    &handleChunkSizeCarriageReturn,
    &handleChunkData,
    &handleChunkDataCarriageReturn,
    &handleChunkDataCrLf
};

void printChar(char c)
{
    if (c == '\n')
        std::cout << CYAN << "/n" << RESET_TEXT << std::endl;
    else if (c == '\r')
        std::cout << CYAN << "/r" << RESET_TEXT << std::endl;
    else
        std::cout << CYAN << c << RESET_TEXT << std::endl;
}

request::ParsingStatus request::handleChunkStart(request& parser, char c) {
        std::cout << RED << parser.currentChunkedState << RESET_TEXT << std::endl;
    switch (parser.currentChunkedState) {

        case Initial: {
            if (c == '\r') {
                parser.currentChunkedState = request::ChunkDataCrLf;
            } else if (isxdigit(c)) {
                parser.currentChunkedState = request::ParsingChunkSize;
            } else {
                parser.error = "err_invalid_chunk_body start";
                return request::ParsingFailed;
            }
            break;
        }
        // Add other cases or handle specific logic related to ChunkedBodyState::Initial

        // Placeholder comment for actual implementation...
        // Implement the logic for ChunkedBodyState::Initial state based on your requirements.

        default:
            // Handle any other necessary logic or error cases
            break;
    }

    return request::ParsingContinue;  // Adjust the return value based on the actual logic
}

// // Parser function implementations for chunked body states
// request::ParsingStatus request::handleChunkStart(request& parser, char c) {
//     std::cout << " debug >> 1" << std::endl;
//     if (c == '\r')
//         parser.currentChunkedState = request::ChunkDataCrLf;
//     else if (isxdigit(c))
//         parser.currentChunkedState = request::ParsingChunkSize;
//     else {
//         parser.error = "err_invalid_chunk_body start";
//         return request::ParsingFailed;
//     }
//     return request::ParsingContinue;
// }


request::ParsingStatus request::handleChunkSize(request& parser, char c) {
    std::cout << " debug >> 2" << std::endl;

    switch (parser.currentChunkedState) {
        case ParsingChunkSize: {
            if (isxdigit(c)) {
                // Update the chunk size based on the hexadecimal digit
                parser.chunkSize = (parser.chunkSize * 0xF + c - '0');
                return request::ParsingContinue;
            } else {
                parser.error = "err_invalid_chunk_body Size";
                return request::ParsingFailed;
            }
        }
        default:
            return request::ParsingFailed;
    }
}

request::ParsingStatus request::handleAfterChunkSizeSpace(request& parser, char c) {
    std::cout << " debug >> 3" << std::endl;
    switch (parser.currentChunkedState) {
        case AfterChunkSizeSpace: {
            if (c == ' ') {
                parser.currentChunkedState = ParsingChunkSize;
                return request::ParsingContinue;
            } else {
                parser.error = "err_invalid_chunk_body space after size";
                return request::ParsingFailed;
            }
        }

        default:
            return request::ParsingFailed;
    }
}

request::ParsingStatus request::handleChunkSizeCarriageReturn(request& parser, char c) {
    std::cout << " debug >> 4" << std::endl;
    switch (parser.currentChunkedState) {
        case ChunkSizeCarriageReturn: {
            if (c == '\r') {
                parser.currentChunkedState = ChunkDataCrLf;
                return request::ParsingContinue;
            } else {
                parser.error = "err_invalid_chunk_body back slash r";
                return request::ParsingFailed;
            }
        }
        default:
            return request::ParsingFailed;
    }
}

request::ParsingStatus request::handleChunkData(request& parser, char c)
{
    std::cout << " debug >> 5" << std::endl;
    if (parser.currentChunkedState == ParsingChunkData)
    {
        std::cout << "entered che=unked data : " << c << std::endl;
        // Note: Adjust the filename and the path as needed
        std::ofstream outputFile("output", std::ios::app);
        if (outputFile.is_open())
        {
            outputFile << c;
            outputFile.close();
        }
        else
        {
            // Handle file opening error
            parser.error = "err_file_opening_failed body data";
            return ParsingFailed;
        }
        return ParsingContinue;
    }
    else
    {
        return ParsingFailed;
    }
}

request::ParsingStatus request::handleChunkDataCarriageReturn(request& parser, char c) {
    std::cout << " debug >> 6" << std::endl;
    switch (parser.currentChunkedState) {
        case ChunkDataCarriageReturn: {
            if (c == '\r') {
                parser.currentChunkedState = ChunkDataCrLf;
                return request::ParsingContinue;
            } else {
                parser.error =" err_invalid_chunk_body lhmaq";
                return request::ParsingFailed;
            }
        }
        default:
            return request::ParsingFailed;
    }
}

request::ParsingStatus request::handleChunkDataCrLf(request& parser, char c) {
    std::cout << " debug >> 7" << std::endl;
    switch (parser.currentChunkedState) {
        case ChunkDataCrLf: {
            if (c == '\n') {
                parser.currentChunkedState = Initial;
                return request::ParsingContinue;
            } else {
                parser.error = "err_invalid_chunk_body tango";
                return request::ParsingFailed;
            }
        }
        default:
            return request::ParsingFailed;
    }
}

// Method to parse a body string
request::ParsingStatus request::parseBody(const std::string& body)
{
    // Iterate through each character in the body string
    for (size_t i = 0; i < body.size(); ++i)
    {
        // Get the current character
        char currentChar = body[i];
        printChar(currentChar);
        // std::cout << CYAN << currentChar << RESET_TEXT << std::endl;

        // Call the appropriate parser function based on the current state
        ParserFunction parserFunction = chunkedStateHandlers[currentChunkedState];
        ParsingStatus status = parserFunction(*this, currentChar); 
        // Check the parsing status
        std::cout  << status << std::endl;
        switch (status)
        {
        case ParsingContinue:
            // Continue parsing
            break;
        case ParsingFailed:
            return ParsingFailed;
        case ParsingDone:
            // Example: Reset state to Initial for the next chunk
            currentChunkedState = Initial;
            break;
        }
    }
    // If we reach here, the parsing is incomplete, and more data is needed
    return ParsingContinue;
}

// void request::checkBody(std::string body)
// {
//     // Create an instance of your bodyParser class
//     for (size_t i = 0; i < body.size(); ++i)
//     {
//         char currentChar = body[i];
//         printChar(currentChar);
//         // std::cout << currentChar << std::endl;
//     }
// }

request::ParsingStatus request::parsChunked(char c)
{
    std::cout << GREEN << currentChunkedState << RESET_TEXT << std::endl;
    printChar(c);
    switch (currentChunkedState) {
    case Initial: {
        std::cout << WHITE << "debug >> 1" << RESET_TEXT << std::endl;
        if (c == '\r') {
            currentChunkedState = request::ChunkDataCrLf;
        }
        else if (isxdigit(c)) {
            currentChunkedState = request::ParsingChunkSize;
        } 
        else {
            // error = err_invalid_chunk_body;
            return request::ParsingFailed;
        }
    }
    case ParsingChunkSize: {
        std::cout << WHITE << "debug >> 2" << RESET_TEXT << std::endl;
        if (isxdigit(c)) {
            chunkSize = (chunkSize * 0xF + c - '0');
            std::cout << RED << chunkSize << RESET_TEXT << std::endl;
            break;
        }

    }
    case AfterChunkSizeSpace: {
        std::cout << WHITE << "debug >> 3" << RESET_TEXT << std::endl;
        if (c == ' ' || c == '\t')
            break;
        if (c == '\r') {
            currentChunkedState = request::ChunkSizeCarriageReturn;
            break;
        } else if (c == ';')
        {
            // error = err_invalid_chunk_body;
            return request::ParsingFailed; // specific error should be returned
        }
        else
            return request::ParsingFailed;
    }
    case ChunkSizeCarriageReturn: {
        std::cout << WHITE << "debug >> 4" << RESET_TEXT << std::endl;
        if (c == '\n') {
            currentChunkedState = request::ParsingChunkData;
            break;
        } else
        {
            // error = err_invalid_chunk_body;
            return request::ParsingFailed;
        }
    }
    case ParsingChunkData: {
        std::cout << WHITE << "debug >> 5" << RESET_TEXT << std::endl;
        std::cout << WHITE << chunkSize << RESET_TEXT << std::endl;
        if (chunkSize == 0) {
            return request::ParsingDone;
        }
        chunkSize--;
        // chunk.push_back(c);
        // error += c;
        // std::cout << YELLOW << c << RESET_TEXT << std::endl;
        std::ofstream outputFile("output", std::ios::app);

        if (outputFile.is_open())
        {
            outputFile << c;
            outputFile.close();
        }
        else
        {
            // Handle file opening error
            // parser.error = "err_file_opening_failed body data";
            std::cout << RED << "Laaaaa" << RESET_TEXT << std::endl;
            return ParsingFailed;
        }
        if (chunkSize == 0) {
            currentChunkedState = request::ChunkDataCarriageReturn;
        }
        break;
    }
    case ChunkDataCarriageReturn: {
        std::cout << WHITE << "debug >> 6" << RESET_TEXT << std::endl;
        if (c != '\r')
        {
            // error = err_invalid_chunk_body;
            return request::ParsingFailed;
        }
        currentChunkedState = request::ChunkDataCrLf;
        break;
    }
    case ChunkDataCrLf: {
        std::cout << WHITE << "debug >> 7" << RESET_TEXT << std::endl;
        if (c != '\n')
        {
            // error = err_invalid_chunk_body;
            return request::ParsingFailed;
        }
        currentChunkedState = request::Initial;
        break;
    }
}
return request::ParsingContinue;
    // std::cout << currentChar << std::endl;
}

request::ParsingStatus request::checkBody(std::string body)
{
    // Create an instance of your bodyParser class
    for (size_t i = 0; i < body.size(); ++i)
    {
        char currentChar = body[i];
        // printChar(currentChar);
        parsChunked(currentChar);
    }
    return ParsingDone;
}

void request::parseRequest(std::string request)
{
    checkRequestLine(request);
    std::cout << request.substr(0, request.find("\r\n\r\n")) << std::endl;
    
    checkHeaderFields(request.substr(0, request.find("\r\n\r\n")));

    for (std::map<std::string, std::string>::iterator it = request::headerFields.begin(); it != request::headerFields.end(); ++it) {
        std::cout << "|" << it->first << "|" << it->second << "|" << std::endl;
    }

    std::cout << RED << request.substr(request.find("\r\n\r\n")) << RESET_TEXT << std::endl;
    // parseBody(request.substr(request.find("\r\n\r\n")));
    chunkSize = 0;
    checkBody(request.substr(request.find("\r\n\r\n") + 4));
    std::cout << RED << error << RESET_TEXT << std::endl;
    std::cout << RED << chunkSize << RESET_TEXT << std::endl;
}

// void request::checkBody(std::string body)
// {
//     // std::istringstream iss(body);
//     // std::string line;
//     // std::ostringstream actualBody;

//     // while (std::getline(iss, line))
//     // {
//     //     // Convert the chunk size from hexadecimal to an integer
//     //     std::size_t chunkSize;
//     //     std::istringstream(line) >> std::hex >> chunkSize;

//     //     // If the chunk size is 0, it's the last chunk, and we stop processing
//     //     if (chunkSize == 0)
//     //         break;

//     //     // Read the chunk data
//     //     std::string chunkData;
//     //     chunkData.resize(chunkSize);
//     //     iss.read(&chunkData[0], static_cast<std::streamsize>(chunkSize));

//     //     // Ignore the CRLF following the chunk data
//     //     iss.ignore(2);

//     //     // Append the chunk data to the actual body
//     //     actualBody << chunkData;
//     // }

//     // std::cout << "Actual Body: " << actualBody.str() << std::endl;
// }

// // Method to parse a body string
// request::ParsingStatus request::parseBody(const std::string& body)
// {
//     // Iterate through each character in the body string
//     for (size_t i = 0; i < body.size(); ++i)
//     {
//         // Get the current character
//         char currentChar = body[i];

//         std::cout << "|" << currentChar << "|" << std::endl;
//         std::cout  << chunkSize << std::endl;
//         // Call the appropriate parser function based on the current state
//         ParserFunction parserFunction = chunkedStateHandlers[currentChunkedState];
//         parserFunction(*this, currentChar);
//         // ParsingStatus status = 
//         // // Check the parsing status
//         // std::cout  << status << std::endl;
//         // switch (status)
//         // {
//         // case ParsingContinue:
//         //     // Continue parsing
//         //     break;
//         // case ParsingFailed:
//         //     // Parsing failed, handle error
//         //     // error = "err_invalid_chunk_body";
//         //     return ParsingFailed;
//         // case ParsingDone:
//         //     // Parsing is done, reset state or perform further actions
//         //     // Example: Reset state to Initial for the next chunk
//         //     currentChunkedState = Initial;
//         //     break;
//         // }
//     }

//     // If we reach here, the parsing is incomplete, and more data is needed
//     return ParsingContinue;
// }

// // Method to parse a body string
// request::ParsingStatus request::parseBody(const std::string& body)
// {
//     // Iterate through each character in the body string
//     for (size_t i = 0; i < body.size(); ++i)
//     {
//         // Get the current character
//         char currentChar = body[i];

//         std::cout << "|" << currentChar << "|" << std::endl;
//         std::cout  << chunkSize << std::endl;
//         // Call the appropriate parser function based on the current state
//         ParserFunction parserFunction = chunkedStateHandlers[currentChunkedState];
//         parserFunction(*this, currentChar);
//         // ParsingStatus status = 
//         // // Check the parsing status
//         // std::cout  << status << std::endl;
//         // switch (status)
//         // {
//         // case ParsingContinue:
//         //     // Continue parsing
//         //     break;
//         // case ParsingFailed:
//         //     // Parsing failed, handle error
//         //     // error = "err_invalid_chunk_body";
//         //     return ParsingFailed;
//         // case ParsingDone:
//         //     // Parsing is done, reset state or perform further actions
//         //     // Example: Reset state to Initial for the next chunk
//         //     currentChunkedState = Initial;
//         //     break;
//         // }
//     }

//     // If we reach here, the parsing is incomplete, and more data is needed
//     return ParsingContinue;
// }

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

string request::getrequestURI(){
    return requestURI;
}

string request::getContentType(){
    return this->ContentType;
}
