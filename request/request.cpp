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
	this->redirectURL = "";
	this->statusCode = "200 OK";
	this->failHeader = false;
	this->keepAlive = false;
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
	if (fileExists(filePath.c_str()))
		unlink(filePath.c_str());
	std::ofstream outputFile(filePath, std::ios::trunc);

	if (outputFile.is_open())
	{
		outputFile << "<!DOCTYPE html>\n";
		outputFile << "<html>\n";
			outputFile << "<head>\n";
				outputFile << "<style>\n";
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
	
		outputFile.close();
		return filePath;
	}
	else
	{
		std::cout << "error" << std::endl;
		std::string filePath = "/Users/zbouayya/goinfre/errorpage.html";
		if (fileExists(filePath.c_str()))
			unlink(filePath.c_str());
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

			outputFile.close();
		}
		return filePath;
	}
		outputFile << "</html>\n";
	
		outputFile.close();
		return filePath;
	}
	else
	{
		std::cout << "error" << std::endl;
		std::string filePath = "/Users/zbouayya/goinfre/errorpage.html";
		if (fileExists(filePath.c_str()))
			unlink(filePath.c_str());
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

			outputFile.close();
		}
		return filePath;
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

	stream2 >> this->method >> this->requestURI >> this->httpVersion;

	if (this->method.empty() || this->requestURI.empty() || this->httpVersion.empty()) {
		this->statusCode = "400 Bad Request";
		this->filePath = errorPageTamplate("400, Bad Request");
		return 1;
	}

	this->requestURI = removeAndSetQueryString(this->requestURI);
	if (this->method != "GET" && this->method != "POST" && this->method != "DELETE") {
		this->statusCode = "405 Method Not Allowed";
		this->filePath = errorPageTamplate("405, Method Not Allowed.");
		return 1;
	}
	if (this->requestURI.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%") != std::string::npos) {
		this->statusCode = "400 Bad Request";
		this->filePath = errorPageTamplate("400, Bad Request");
		return 1;
	}
	if (this->requestURI.size() > 2048)/* mazal request body larger than lbody li fl config file !!*/ {
		this->statusCode = "414 Bad Request";
		this->filePath = errorPageTamplate("414, Bad Request");
		return 1;
	}
	if (this->httpVersion != "HTTP/1.1") {
		this->statusCode = "505 HTTP Version Not Supported";
		this->filePath = errorPageTamplate("505, HTTP Version Not Supported");
		return 1;
	}
	return 0;
}

int request::checkHeaderFields(std::string headerFiles)
{
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

std::string getTheExtensionFromContentType(std::string daContentType)
{
	// Find the position of the '/'
	size_t slashPos = daContentType.find('/');

	if (slashPos != std::string::npos) {
		// Extract the substring after the '/'
		std::string fileType = daContentType.substr(slashPos + 1);

		// Replace any '+' characters with '_' (e.g., image/jpeg -> jpeg)
		size_t plusPos = fileType.find('+');
		while (plusPos != std::string::npos) {
			fileType[plusPos] = '_';
			plusPos = fileType.find('+', plusPos + 1);
		}
		// Return the file extension
		return "." + fileType;
	} else {
		// Return an empty string if no '/' is found
		return "";
	}
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
	std::string fileName;
	if ((int)body.size() > _server.getClientBodyLimit()) {
		this->statusCode = "413 Request Entity Too Large";
		this->filePath = errorPageTamplate("413, Request Entity Too Large");
		return ParsingFailed;
		printError("Request Entity Too Large", 413);
	}
	std::istringstream stream(body);
	while (true) {
		std::string chunkSizeHex;
		std::getline(stream, chunkSizeHex);

		// Convert hex string to integer
		// std::size_t chunkSize = std::stoul(chunkSizeHex, nullptr, 16);
		std::istringstream iss(chunkSizeHex);
		std::size_t chunkSize;
		
		iss >> std::hex >> chunkSize;

		if (iss.fail()) {
			std::cerr << "Conversion failed.\n";//! dir chi haja
			// return 1;
		}
		// Check for the end of chunks
		if (chunkSize == 0) {
			break;
		}

		// Read the chunk data
		std::string chunkData(chunkSize, '\0');
		stream.read(&chunkData[0], chunkSize);

		// Consume the CRLF at the end of the chunk
		std::string crlf;
		std::getline(stream, crlf);
		fileName = generateRandomFileName() + getTheExtensionFromContentType(this->uploadContentType);
		std::ofstream outputFile("upload/" + fileName, std::ios::app);
		outputFile << chunkData;
		if (outputFile.fail()) {
			std::cerr << RED << "uploading failed !!!!!!!!!!" << RESET_TEXT << endl;
			outputFile.close();
			this->statusCode = "507 Insufficient Storage";
			this->filePath = errorPageTamplate("507, Insufficient Storage.");
			this->ContentType = "text/html";
			return ParsingFailed;
		}
		outputFile.close();
		if (outputFile.fail()) {
			std::cerr << RED << "uploading failed !!!!!!!!!!" << RESET_TEXT << endl;
			outputFile.close();
			this->statusCode = "507 Insufficient Storage";
			this->filePath = errorPageTamplate("507, Insufficient Storage.");
			this->ContentType = "text/html";
			return ParsingFailed;
		}
		outputFile.close();
	}
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
	fileName = generateRandomFileName() + getTheExtensionFromContentType(this->uploadContentType);
	std::ofstream outputFile("upload/" + fileName, std::ios::app);
	if (outputFile.is_open()) {
		cout << "file is open" << endl;
		outputFile << body;
		if (outputFile.fail()) {
			std::cerr << RED << "uploading failed !!!!!!!!!!" << RESET_TEXT << endl;
			outputFile.close();
			this->statusCode = "507 Insufficient Storage";
			this->filePath = errorPageTamplate("507, Insufficient Storage.");
			this->ContentType = "text/html";
			return ParsingFailed;
		}
		if (outputFile.fail()) {
			std::cerr << RED << "uploading failed !!!!!!!!!!" << RESET_TEXT << endl;
			outputFile.close();
			this->statusCode = "507 Insufficient Storage";
			this->filePath = errorPageTamplate("507, Insufficient Storage.");
			this->ContentType = "text/html";
			return ParsingFailed;
		}
	}
	if (!outputFile.is_open()) {
		cout << "file not open" << endl;
	}
	outputFile.close();
	bodyContentLength += body.size();
	return ParsingDone;
}

std::string extractFilename(const std::string& boundaryHeaders) {
	std::string filename;
	size_t filenamePos = boundaryHeaders.find("filename=\"");
	
	if (filenamePos != std::string::npos) {
		filenamePos += 10;  // Move to the start of the filename
		size_t endPos = boundaryHeaders.find("\"", filenamePos);

		if (endPos != std::string::npos) {
			filename = boundaryHeaders.substr(filenamePos, endPos - filenamePos);
		}
	}
	else
		filename = "newname";
	return filename;
}

bool fileExists2(const std::string& fileName) {
    std::ifstream file(fileName.c_str());
    return file.is_open();
}

std::string generateNewFileName(const std::string& fileName) {
	size_t dotPosition = fileName.find_last_of('.');
	std::string baseName;
	std::string extension;
	if (dotPosition != std::string::npos) {
		baseName = fileName.substr(0, dotPosition);
		extension = fileName.substr(dotPosition);
	}
	else {
		baseName = fileName;
		extension = "";
	}

	int counter = 2;
	std::string newFileName;
	newFileName = fileName;
	bool exist = fileExists2("upload/" + fileName);
	while (exist)
	{
		std::ostringstream oss;
		oss << baseName << counter << extension;
		newFileName = oss.str();
		exist = fileExists2("upload/" + newFileName);
		counter++;
	}

	return newFileName;
}

request::ParsingStatus request::checkBody3(std::string body, server& _server)
{
	if ((int)body.size() > _server.getClientBodyLimit()) {
		this->statusCode = "413 Request Entity Too Large";
		this->filePath = errorPageTamplate("413, Request Entity Too Large");
		return ParsingFailed;
		printError("Request Entity Too Large", 413);
	}
	size_t pos = 0;
	size_t nextPos = 0;
	while ((nextPos = body.find(boundary, pos)) != std::string::npos)
	{
		std::string part = body.substr(pos, nextPos - pos);
		if (part.find("\r\n\r\n") != std::string::npos)
		{
			std::string fileName = extractFilename(part.substr(0, part.find("\r\n\r\n")));
			fileName = generateNewFileName(fileName);
			std::ofstream outputFile("upload/" + fileName);
			if (outputFile.is_open())
			{
				std::string bodySent = part.substr(part.find("\r\n\r\n") + 4);
				if (bodySent.empty() || bodySent == "\r\n") {
					unlink(("upload/" + fileName).c_str());
				}
				outputFile << bodySent.substr(0, bodySent.size() - 2);
				if (outputFile.fail()) {
					std::cerr << RED << "uploading failed !!!!!!!!!!" << RESET_TEXT << endl;
					outputFile.close();
					this->statusCode = "507 Insufficient Storage";
					this->filePath = errorPageTamplate("507, Insufficient Storage.");
					this->ContentType = "text/html";
					return ParsingFailed;
				}
				outputFile.close();
				if (outputFile.fail()) {
					std::cerr << RED << "uploading failed !!!!!!!!!!" << RESET_TEXT << endl;
					outputFile.close();
					this->statusCode = "507 Insufficient Storage";
					this->filePath = errorPageTamplate("507, Insufficient Storage.");
					this->ContentType = "text/html";
					return ParsingFailed;
				}
				outputFile.close();
			}
		}
		pos = nextPos + boundary.length();
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
	return (S_ISREG(fileInfo.st_mode) || S_ISDIR(fileInfo.st_mode));
}

std::string removewhites(const std::string& str) {
	size_t start = str.find_first_not_of(" \t\r\n");
	size_t end = str.find_last_not_of(" \t\r\n");
//!leaks here??
	if (start == std::string::npos || end == std::string::npos) {
		return "";  // String is all whitespaces
	}
	return str.substr(start, end - start + 1);
}

int request::matchLocation(server& _server)
{
	std::vector<Location> vec;
	vec = _server.getLocations();
	std::string paths = this->requestURI;

	// std::cout << MAGENTA << "***** the path *****" << paths << RESET_TEXT << std::endl;

	// if (fileExists(paths.c_str()))
	// {
	// 	std::cout << GREEN << "***** flbla *****" << RESET_TEXT << std::endl;
	// 	filePath = this->requestURI;
	// 	if (filePath[0] == '/')
	// 		filePath = filePath.substr(1);
	// 	return 0;
	// }
	
	std::vector<server> srvrs = _server.getSmSoServers();//? servers li wst servers
	// //? lopping throught a vector of servers
	for (std::vector<server>::iterator it = srvrs.begin(); it != srvrs.end(); it++) {
		// cout<< MAGENTA << "server name is: " << it->getServerName() << RESET_TEXT << endl;

		// cout<< MAGENTA << "Host is: " << removewhites(this->headerFields["Host"]) << "|" << RESET_TEXT << endl;
		if (it->getServerName() == removewhites(this->headerFields["Host"]))
		{
			vec = it->getLocations();
			// cout << BLUE << "YES WE FOUND A MATCH: " << it->getServerName() << RESET_TEXT << endl;
			//location tbedel ldak dyal server lakhor
		}
		// ila la khliha dyal default
	}

	while (!paths.empty())
	{
		// std::cout << GREEN << "***** looping *****" << RESET_TEXT << std::endl;
		for (std::vector<Location>::iterator it = vec.begin(); it != vec.end(); it++) {
			// std::cout << RED << "is LOCATION: " << it->getLocationName() << "\t\tequal to URI: " << paths << RESET_TEXT << std::endl;
			if (it->getLocationName() == paths) {
				// std::cout << GREEN << "*****FOUND A MATCH*****" << RESET_TEXT << std::endl;
				filePath = it->getRoot() + this->requestURI;
				if (!it->getRedirection().empty()) {
					redirectURL = it->getRedirection();
				}
				if (isDirectory(filePath.c_str())) {
				// std::cout << GREEN << "*****FOUND A MATCH 22 *****" << RESET_TEXT << std::endl;
					if (filePath[filePath.size() - 1] == '/')
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
	if (request.empty()) {//???????? useless maybe ?
		return 1;
	}
	if (headerFields.find("Transfer-Encoding") != headerFields.end()) {
		cout << MAGENTA << "UPLOAD WITH CHuNKED <>" << RESET_TEXT << endl;
		checkBody(request, _server);
	}
	else {
		if (headerFields.find("Content-Type") != headerFields.end()
		&& headerFields["Content-Type"].find("multipart/form-data") != std::string::npos) {
			cout << MAGENTA << "UPLOAD WITH BOuNDARY <>" << RESET_TEXT << endl;
			checkBody3(request, _server);
			std::string extension = getFileExtension(this->requestURI);
			if (!extension.empty())
				this->ContentType = allContTypes[extension];
		}
		else {
			cout << MAGENTA << "UPLOAD WITH CONTENT LENGTH <>" << RESET_TEXT << endl;
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


bool	request::getConnection()
{
	return this->keepAlive;
}

void	request::setConnection()
{
	if (headerFields.find("Connection") != headerFields.end())
	{
		if (headerFields["Connection"].find("keep-alive") != std::string::npos) {
			this->keepAlive = true;
		}
		else {
			this->keepAlive = false;
		}
	}
}

void request::setContentType()
{
	addAllContentTypes();

	if (this->method == "GET" || this->method == "POST") {
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
		if (this->headerFields.count("Content-Type") > 0 && !this->headerFields["Content-Type"].empty()){
			this->uploadContentType = this->headerFields["Content-Type"];
			this->boundary = "--" + this->uploadContentType.substr(this->uploadContentType.find("boundary=") + 9);
			size_t lastNonSpace = this->boundary.find_last_not_of(" \t\r\n");
			if (lastNonSpace != std::string::npos) {
				this->boundary.erase(lastNonSpace + 1);
			}
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
		this->headers.append(requestPart.substr(0, requestPart.find("\r\n\r\n")));
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