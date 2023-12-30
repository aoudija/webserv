#include "../server.hpp"

// std::string request::requestURI;
// std::string request::httpVersion;
// std::string request::method;
// std::map<std::string, std::string> request::headerFields;
using std::string;
using std::cout;
using std::endl;

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

void request::setContentType()
{
    std::string fileExtension;
    size_t dotPosition = requestURI.rfind(".");

    if (dotPosition != std::string::npos) {
        fileExtension = requestURI.substr(dotPosition);
    }
    else {
        cout << RED << requestURI << RESET_TEXT << endl;
        std::cerr << "Error: No dot found in requestURI\n";
    }
    this->ContentType = allContTypes[fileExtension];
}

void request::checkRequestLine(std::string request)
{
    std::istringstream stream(request);
    std::string line;
    std::getline(stream, line);

    std::istringstream stream2(line);

    // std::cout << line << std::endl;
    // request req;
    // std::string method, requestURI, httpVersion;

    stream2 >> this->method >> this->requestURI >> this->httpVersion;
    if (this->method != "GET" && this->method != "POST" && this->method != "DELETE")
        printError("Method Not Allowed", 405);
    // if (this->requestURI.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%"))
    //     printError("Bad Request", 400);
    if (this->requestURI.size() > 2048)// mazal request body larger than lbody li fl config file !!
        printError("Request-URI Too Long", 414);
    if (this->httpVersion != "HTTP/1.1")
        printError("HTTP Version Not Supported", 505);
    setContentType();
    // std::cout << this->requestURI.substr(this->requestURI.rfind(".")) << std::endl;
    
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
    // std::cout << " \\\\\\ " << std::endl;
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

void request::parseRequest(std::string request)
{
    addAllContentTypes();
    checkRequestLine(request);
    // std::cout << request.substr(0, request.find("\r\n\r\n")) << std::endl;
    
    checkHeaderFields(request.substr(0, request.find("\r\n\r\n")));

    // for (std::map<std::string, std::string>::iterator it = request::headerFields.begin(); it != request::headerFields.end(); ++it) {
    // std::cout << it->first << ": " << it->second << std::endl;
    // }

    // std::cout << "\033[1;33m" << request.substr(request.find("\r\n\r\n")) << std::endl;
    // checkBody(request.substr(request.find("\r\n\r\n")));
    std::cout << RED << getContentType() << RESET_TEXT << std::endl;
}

string request::getrequestURI(){
    return requestURI;
}

string request::getContentType(){
    return this->ContentType;
}
