#include "../server.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

request::request() : requestStatus(true) {
	this->chunkSize = 0;
	this->bodyContentLength = 0;
	this->bodyDone = 0;
	this->headersDone = 0;
	this->flag = 0;
	this->gg = 0;
	this->is_CGI = 0;
}

// request::request()
// {
//	 this->chunkSize = 0;
//	 // this->_request = _request;
//	 // this->_server = _server;
//	 // parseRequest(request, _server);
// }

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

string request::getrequestURI() {
	return this->requestURI;
}

string request::getredirectURL() {
	return this->redirectURL;
}

string request::getContentType() {
	return this->ContentType;
}

std::string request::getHttpVersion() {
	return this->httpVersion;
}

std::string request::getFilePath() {
	return this->filePath;
}

void request::setFilePath(std::string filePath) {
	this->filePath = filePath;
}

bool	request::isRequestDone() {
	return requestStatus;
}

std::string request::getStatusCode() {
    return this->statusCode;
}

void request::setStatusCode(std::string statusCode) {
	this->statusCode = statusCode;
}

string request::getQueryString() {
	return this->queryString;
}

std::string errorPageTamplate(std::string errorMessage)
{
    std::string filePath = "errorpage.html";
    std::ofstream outputFile(filePath, std::ios::trunc);

    if (outputFile.is_open())
    {
        outputFile << "<!DOCTYPE html>\n";
        outputFile << "<html>\n";
        	outputFile << "<head>\n";
        		outputFile << "<style>\n";
					outputFile << "html, body {font-family: 'Roboto Mono', monospace;font-size: 16px;}\n";
					outputFile << "body {background-color: black;margin: 0;padding: 0;}\n";
					outputFile << "p {color: white;font-size: 25px;letter-spacing: 0.2px;margin: 0;display: inline;}\n";
					outputFile << ".center-xy {text-align: center;top: 50%;left: 50%;transform: translate(-50%, -50%);position: absolute;}\n";
				outputFile << "</style>\n";
			outputFile << "</head>\n";

			outputFile << "<body>\n";
				outputFile << "<div class='center-xy'>\n";
					outputFile << "<p id='myP'>\n";
						outputFile << errorMessage + "\n";
					outputFile << "</p>\n";
				outputFile << "</div>\n";
				outputFile << "<script>\n";

				outputFile <<
						"let divElement = document.getElementById(\"myP\");"
						"let textContent = divElement.innerText.toString();"
						"let i = 1;"
						"function typeWriter() {"
						"	divElement.innerText = textContent.slice(0, i);"
						"	console.log(divElement.innerText);"
						"	i++;"
						"	if (i <= textContent.length)"
						"		setTimeout(typeWriter, 100);"
						"}"
						"setTimeout(typeWriter, 0);\n";
				outputFile << "</script>\n";
			outputFile << "</body>\n";
        outputFile << "</html>\n";



        // writeToFile(".container { width: 100%;}\n");


        

        // outputFile << "<div class='container'>\n";
        // outputFile << "<div class='copy-container center-xy'>\n";
        // outputFile << "<p>\n";
        // outputFile << errorMessage + "\n";

        // outputFile << "</div>\n";
        // writeToFile("</div>\n");

        outputFile.close();
        return filePath;
    }
    else
    {
        std::cout << "error" << std::endl;
        return "";
    }
}

std::string request::removeAndSetQueryString(const std::string& uri) {

	std::string::size_type queryStringPos = uri.find('?');

	if (queryStringPos != std::string::npos) {
		this->queryString = uri.substr(queryStringPos + 1);
		return uri.substr(0, queryStringPos);
	}
	this->queryString = "";
	return uri;
}

int request::checkRequestLine(std::string request)
{
	std::istringstream stream(request);
	std::string line;
	std::getline(stream, line);

	std::istringstream stream2(line);

	// std::cout << line << std::endl;

	stream2 >> this->method >> this->requestURI >> this->httpVersion;
	this->requestURI = removeAndSetQueryString(this->requestURI);
	if (this->method != "GET" && this->method != "POST" && this->method != "DELETE") {
		this->statusCode = "405 Method Not Allowed";
		this->filePath = errorPageTamplate("405, Method Not Allowed.");
		return 1;
		printError("Method Not Allowed", 405);
	}
	if (this->requestURI.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%") != std::string::npos) {
		this->statusCode = "400 Bad Request";
		this->filePath = errorPageTamplate("400, Bad Request");
		return 1;
		printError("Bad Request", 400);
	}
	if (this->requestURI.size() > 2048)/* mazal request body larger than lbody li fl config file !!*/ {
		this->statusCode = "414 Bad Request";
		this->filePath = errorPageTamplate("414, Bad Request");
		return 1;
		printError("Bad Request", 414);
	}
	if (this->httpVersion != "HTTP/1.1") {
		this->statusCode = "505 HTTP Version Not Supported";
		this->filePath = errorPageTamplate("505, HTTP Version Not Supported");
		return 1;
		printError("HTTP Version Not Supported", 505);
	}
	return 0;
}

int request::checkHeaderFields(std::string headerFiles)
{
	// std::cout << request::method << std::endl;

	std::string line;
	std::vector<std::string> lines;
		
	std::istringstream iss(headerFiles);

	while (std::getline(iss, line)) {
		lines.push_back(line);
	}

	for (std::vector<std::string>::iterator i = lines.begin(); i != lines.end(); i++) {
		if (i->find(":") != std::string::npos) {
			this->headerFields[i->substr(0, i->find(":"))] = i->substr(i->find(":") + 2);
		}
	}
	if (headerFields.find("Transfer-Encoding") != headerFields.end()
		&& headerFields["Transfer-Encoding"].find("chunked") == std::string::npos) {
		this->statusCode = "501 Not implemented";
		this->filePath = errorPageTamplate("501, Not implemented");
		return 1;
		printError("Not implemented", 501);
	}
	if (headerFields.find("Transfer-Encoding") == headerFields.end()
		&& headerFields.find("Content-Length") == headerFields.end()
		&& this->method == "POST") {
		this->statusCode = "400 Bad Request";
		this->filePath = errorPageTamplate("400, Bad Request");
		return 1;
		printError("Bad Request", 400);
	}
	// if (getMethod() == "GET") {
	// 	cout << "ra dart mn hna" << endl;
	// 	return 1;
	// }
	return 0;
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
	// cout << RED << "the char id: " << c << RESET_TEXT << endl;
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
			chunkSize = (chunkSize * 16) + (isdigit(c) ? (c - '0') : (toupper(c) - 'A' + 10));
			// cout << "the c num is: " << c << " and chunksize is: " << chunkSize << endl;
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
		// cout << "ja hna  +++++++++++++++++++++" << chunkSize << endl;
		chunkSize--;
		std::ofstream outputFile("theBodyContentIsHere", std::ios::app);

			// cout << "ja hna +++++++++++++++++++++" << endl;
		if (outputFile.is_open() && this->method != "GET")
		{
			// cout << "dsjfhh" << endl;
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

std::string generateRandomFileName() {
    std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int length = 8;  // Adjust the length as needed

    std::string randomFileName;
    srand(static_cast<unsigned>(time(0)));

    for (int i = 0; i < length; ++i) {
        int randomIndex = rand() % characters.length();
        randomFileName += characters[randomIndex];
    }

    return randomFileName;
}

request::ParsingStatus request::checkBody(std::string body, server& _server)
{
	if ((int)body.size() > _server.getClientBodyLimit()) {
		this->statusCode = "413 Request Entity Too Large";
		this->filePath = errorPageTamplate("413, Request Entity Too Large");
		return ParsingFailed;
		printError("Request Entity Too Large", 413);
	}
	if (chunkSize == 0)
		currentChunkedState = Initial;
	else if (chunkSize > 0)
		currentChunkedState = ParsingChunkData;
	// cout << BLUE << "currentChunkedState is: " << currentChunkedState << " and chunk size is: "<< chunkSize <<"and c= " << RESET_TEXT << endl;
	for (size_t i = 0; i < body.size(); ++i)
	{
		char currentChar = body[i];
		ParsingStatus status = parsChunked(currentChar);
		switch (status)
		{
		case ParsingContinue:
			// cout << RED << "the parsing continue" << RESET_TEXT << endl;
			break;// Continue parsing
		case ParsingFailed:
			// cout << RED << "the parsing failed" << RESET_TEXT << endl;
			return ParsingFailed;
		case ParsingDone:
			// cout << RED << "the parsing is done 1" << RESET_TEXT << endl;
			currentChunkedState = Initial;// Reset state to Initial for the next chunk
			break;
		}
	}
	// cout << RED << "the parsing is done 2" << RESET_TEXT << endl;
	return ParsingDone;
}

request::ParsingStatus request::checkBody2(std::string body, server& _server)
{
	if ((int)body.size() > _server.getClientBodyLimit()) {
		this->statusCode = "413 Request Entity Too Large";
		this->filePath = errorPageTamplate("413, Request Entity Too Large");
		return ParsingFailed;
		printError("Request Entity Too Large", 413);
	}
	std::string fileName;
	fileName = generateRandomFileName();
	cout << "the content type is:" << this->ContentType << endl;
	std::ofstream outputFile(filename, std::ios::app);
	if (outputFile.is_open() && this->method != "GET") {
		outputFile << body;
		outputFile.close();
	}
	bodyContentLength += body.size();
	return ParsingDone;
}

request::ParsingStatus request::checkBody3(std::string body, server& _server)
{
	// cout << RED << "ENTER " << boundary << RESET_TEXT << endl;
	// cout << BLUE << "BODY " << body << RESET_TEXT << endl;
	if ((int)body.size() > _server.getClientBodyLimit()) {
		this->statusCode = "413 Request Entity Too Large";
		this->filePath = errorPageTamplate("413, Request Entity Too Large");
		return ParsingFailed;
		printError("Request Entity Too Large", 413);
	}
	// std::istringstream iss(string(&body[0], &body[0] + body.size()));
	std::istringstream iss(body);

	std::string line;
	// std::string filename;

    while (getline(iss, line)) {
		if (line.find(boundary) != std::string::npos && flag == 0) {
			getline(iss, line);// skip boundary
			flag = 1;
		}
		// if (line.compare(boundary + "--") == 0) {
		// 		// end of file upload
		// 		break;
		// }
		if (line.find("Content-Disposition:") != std::string::npos)
		{
			bodyContentLength++;//? added to handel when there is no body
			gg = 1;
			if (line.find("filename=\"") != std::string::npos) {
				size_t filename_start = line.find("filename=\"") + 10;
				size_t filename_end = line.find('\"', filename_start);
				if (filename_start != std::string::npos && filename_end != std::string::npos) {
					filename = line.substr(filename_start, filename_end - filename_start);
				}
				// if (iss.tellg() != -1)
					getline(iss, line); // skip Content-Type line
				// if (iss.tellg() != -1)
					getline(iss, line); // skip empty line
			}
			else {
				filename = "form_field.txt";
				getline(iss, line); // skip empty line
			}
		}
		// cout << RED << "ENTER " << filename << RESET_TEXT << endl;
		std::ofstream outputFile("upload/" + filename, std::ios::app);//std::ios::app);
		if (outputFile.is_open()) {
			if (line == "\n")
				getline(iss, line);
			cout << BLACK << line << RESET_TEXT << endl;
			if (gg == 0 || bodyContentLength == 1) {
					outputFile << line + '\n' ;//<< std::endl;
				// if (iss.tellg() != -1)
				// 	outputFile << line + '\n' ;//<< std::endl;
				// else
				// 	outputFile << line;
				// outputFile << std::endl;
				bodyContentLength += line.size() + 1;
			}
			while (getline(iss, line)) {
				gg = 0;
				if (line.find(boundary) != std::string::npos || line.find(boundary + "--") != std::string::npos) {
					flag = 0;
					break;
				}
				if (line == "\r")
					continue;
				cout << BLACK << line << RESET_TEXT << endl;
					outputFile << line + '\n' ;//<< std::endl;
				// if (iss.tellg() != -1)
				// 	outputFile << line + '\n' ;//<< std::endl;
				// else
				// 	outputFile << line;
				bodyContentLength += line.size() + 1;
			}
			outputFile.close();
			if (line.find(boundary + "--") != std::string::npos) {
				bodyContentLength = this->actualContentLength;
			}
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
}

void request::setContentLength()
{
	std::string input = this->headerFields["Content-Length"];
	std::string result;

	for (std::string::iterator it = input.begin(); it != input.end(); ++it) {
		if (std::isdigit(*it)) {
			result += *it;
		}
	}
	std::istringstream(result) >> this->actualContentLength;
	// std::cout << "bytes tedtts  ; " << this->headerFields["Range"] << std::endl;
}

void generatePrefixes(const std::string& path, std::vector<std::string>& prefixes) {
	std::istringstream iss(path);
	std::string component;
	std::string currentPrefix = "/";  // Start with the root "/"

	while (std::getline(iss, component, '/')) {
		if (!component.empty()) {
			currentPrefix += component + "/";  // Add the component to the current prefix
			prefixes.push_back(currentPrefix);
		}
	}
}

bool isDirectory(const char* path)
{
	struct stat fileInfo;

	if (stat(path, &fileInfo) != 0) {
		return false;
	}
	return S_ISDIR(fileInfo.st_mode);// need to check if this is allowed
}

bool fileExists(const char* path) {
	struct stat fileInfo;

	if (path[0] == '/')
		path = path + 1;
	if (stat(path, &fileInfo) != 0) {
		return false;
	}
	return S_ISREG(fileInfo.st_mode);
}

int request::matchLocation(server& _server)
{
	std::vector<Location> vec;
	vec = _server.getLocations();
	std::string paths = this->requestURI;

	// std::cout << MAGENTA << "***** the path *****" << paths << RESET_TEXT << std::endl;

	if (fileExists(paths.c_str()))
	{
		std::cout << GREEN << "***** flbla *****" << RESET_TEXT << std::endl;
		filePath = this->requestURI;
		if (filePath[0] == '/')
			filePath = filePath.substr(1);
		return 0;
	}

	while (!paths.empty())
	{
		// std::cout << GREEN << "***** looping *****" << RESET_TEXT << std::endl;
		for (std::vector<Location>::iterator it = vec.begin(); it != vec.end(); it++) {
			// std::cout << RED << "is LOCATION: " << it->getLocationName() << "\t\tequal to URI: " << paths << RESET_TEXT << std::endl;
			if (it->getLocationName() == paths) {
				std::cout << GREEN << "*****FOUND A MATCH*****" << RESET_TEXT << std::endl;
				filePath = it->getRoot() + this->requestURI;
				if (!it->getRedirection().empty()) {
					redirectURL = it->getRedirection();
				}
				if (isDirectory(filePath.c_str())) {
				std::cout << GREEN << "*****FOUND A MATCH 22 *****" << RESET_TEXT << std::endl;
					filePath = filePath + it->getIndex();
					if (!it->getRedirection().empty())
						redirectURL = it->getRedirection();
				}
				loc = *it;
				// cout << BLUE <<loc.getCgiPath()<< RESET_TEXT << endl;
				if (filePath[0] == '/')
					filePath = filePath.substr(1);
				return 0;
			}
		}
		std::size_t slashLoc = paths.find_last_of('/');
		if (slashLoc != std::string::npos) {
			size_t n = std::count(paths.begin(), paths.end(), '/');
			if (n > 1)
				paths = paths.substr(0, slashLoc);
			else
				paths = "/";
		}
	}
	return 1;
}

int request::parseRequest(std::string request, server& _server)
{
	if (headerFields.find("Transfer-Encoding") != headerFields.end()) {
		checkBody(request, _server);
	}
	else {
		if (headerFields.find("Content-Type") != headerFields.end()
		&& headerFields["Content-Type"].find("multipart/form-data") != std::string::npos) {
			checkBody3(request, _server);
		}
		else {
			checkBody2(request, _server);
		}
	}

	if (headerFields["Transfer-Encoding"].find("chunked") != std::string::npos
		&& headerFields.find("Content-Length") == headerFields.end()
		&& chunkSize != 0) {
			requestStatus = false;
			return 0;
		}
	else if (headerFields["Transfer-Encoding"].find("chunked") == std::string::npos
		&& headerFields.find("Content-Length") != headerFields.end()
		&& bodyContentLength < this->actualContentLength) {
			if (bodyContentLength < this->actualContentLength)
			requestStatus = false;
			return 0;
		}
	else if (headerFields["Transfer-Encoding"].find("chunked") != std::string::npos
		&& headerFields.find("Content-Length") != headerFields.end()
		&& chunkSize != 0) {
			requestStatus = false;
			return 0;
		}
	else {
		return 1;
	}
}

void request::setContentType()
{
	addAllContentTypes();
	if (this->method == "GET") {
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
	if (this->method == "POST") {
		this->ContentType = this->headerFields["Content-Type"];
		this->boundary = "--" + this->ContentType.substr(this->ContentType.find("boundary=") + 9);
		size_t lastNonSpace = this->boundary.find_last_not_of(" \t\r\n");

		if (lastNonSpace != std::string::npos) {
			this->boundary.erase(lastNonSpace + 1);
		}
	}
}

void	request::setContentType(std::string contentType)
{
	this->ContentType = contentType;
}

void	request::addAllContentTypes()
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

int request::getHeadersRequest(std::string requestPart) {
	if (!headersDone && requestPart.find("\r\n\r\n") == std::string::npos) {
		this->headers.append(requestPart.substr(0, requestPart.find("\r\n\r\n")));
		return 0;
	}
	if (!headersDone && requestPart.find("\r\n\r\n") != std::string::npos) {
		// cout << "hhh " << requestPart.substr(0, requestPart.find("\r\n\r\n")) << endl;
		// this->headers = this->headers.append(requestPart.substr(0, requestPart.find("\r\n\r\n")), requestPart.substr(0, requestPart.find("\r\n\r\n")).size());
		// this->headers = requestPart.substr(0, requestPart.find("\r\n\r\n"));
		this->headers.append(requestPart.substr(0, requestPart.find("\r\n\r\n")));
		// this->theBody.append(requestPart.substr(requestPart.find("\r\n\r\n")));
		headersDone = 1;
		return 1;
	}
	return 0;
}


int request::getBodyRequest(std::string requestPart)
{
	if (headersDone) {
		if (headersDone == 3) {
			bodyContentLength += requestPart.substr(requestPart.find("\r\n\r\n") + 4).size();
			this->theBody.append(requestPart.substr(requestPart.find("\r\n\r\n") + 4));
			headersDone = 4;
		}
		else
			this->theBody.append(requestPart);
		if (headerFields.find("Content-Type") != headerFields.end()
			&& headerFields["Content-Type"].find("multipart/form-data") != std::string::npos) {
				if (requestPart.find(boundary + "--") != std::string::npos) {
					cout << WHITE << "out HERE  +++"<< RESET_TEXT << endl;
					return 1;
				}
		}
		if (headerFields.find("Transfer-Encoding") != headerFields.end()
			&& headerFields["Transfer-Encoding"].find("chunked") != std::string::npos) {
				if (requestPart.find("\r\n0\r\n\r\n") != std::string::npos) {
					cout << WHITE << "HERE 1"<< RESET_TEXT << endl;
					return 1;
				}
		}
		else {
			// cout << bodyContentLength<< " == " << this->actualContentLength << endl;
			if ((int)this->theBody.size() >= this->actualContentLength) {
				cout << WHITE << "HERE 2"<< RESET_TEXT << endl;
				return 1;
			}
			// bodyContentLength += requestPart.size();
		}
	}
	return 0;
}


void	request::setCgiHeader(std::string s){
	this->cgi_header = s;
}
void	request::setCgiBody(std::string s){
	this->cgi_body = s;
}

string request::getCgiBody(){
	return this->cgi_body;
}
string request::getCgiHeader(){
	return this->cgi_header;
}