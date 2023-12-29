#include "../server.hpp"

// std::string request::requestURI;
// std::string request::httpVersion;
// std::string request::method;
// std::map<std::string, std::string> request::headerFields;
using std::string;

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

std::string request::getMethod()
{
    return this->method;
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
    if (this->requestURI.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghiklmnopqrstuvwxyz0123456789-_~:/2#0@1$80*+=") != std::string::npos)
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
            std::string key = i->substr(0, i->find(":"));

            std::string value = i->substr(i->find(":") + 1);
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


void request::checkBody(std::string body)
{
    std::istringstream iss(body);
    std::string line;
    std::ostringstream actualBody;

    while (std::getline(iss, line))
    {
        // Convert the chunk size from hexadecimal to an integer
        std::size_t chunkSize;
        std::istringstream(line) >> std::hex >> chunkSize;

        // If the chunk size is 0, it's the last chunk, and we stop processing
        if (chunkSize == 0)
            break;

        // Read the chunk data
        std::string chunkData;
        chunkData.resize(chunkSize);
        iss.read(&chunkData[0], static_cast<std::streamsize>(chunkSize));

        // Ignore the CRLF following the chunk data
        iss.ignore(2);

        // Append the chunk data to the actual body
        actualBody << chunkData;
    }

    std::cout << "Actual Body: " << actualBody.str() << std::endl;
}

void request::parseRequest(std::string request)
{
    checkRequestLine(request);
    std::cout << request.substr(0, request.find("\r\n\r\n")) << std::endl;
    
    checkHeaderFields(request.substr(0, request.find("\r\n\r\n")));

    for (std::map<std::string, std::string>::iterator it = request::headerFields.begin(); it != request::headerFields.end(); ++it) {
    std::cout << it->first << ": " << it->second << std::endl;
    }

    std::cout << "\033[1;33m" << request.substr(request.find("\r\n\r\n")) << std::endl;
    checkBody(request.substr(request.find("\r\n\r\n")));
}

string request::getrequestURI(){
    return requestURI;
}

string request::getContentType(){
    return headerFields["Content-Type:"];
}
