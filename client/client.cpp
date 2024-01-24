#include "../server.hpp"
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

bool endsWithSlash(const std::string& str) {
    if (str.length() == 0)
        return false;

    char lastChar = str[str.length() - 1];
    return lastChar == '/';
}

void generateAutoIndex(const std::string& directoryPath, const std::string& outputFileName) {
    DIR* dir = opendir(directoryPath.c_str());

    if (!dir) {
        std::cerr << "Error opening directory: " << strerror(errno) << std::endl;
        return;
    }

    std::ofstream outputFile(outputFileName.c_str());

    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file: " << strerror(errno) << std::endl;
        closedir(dir);
        return;
    }

    // Write HTML header
    outputFile << "<html><head><title>Index of " << directoryPath << "</title></head><body>\n";
    outputFile << "<h2>Index of " << directoryPath << "</h2>\n";
    outputFile << "<ul>\n";

    // Read directory contents
    struct dirent* entry;

    while ((entry = readdir(dir)) != NULL) {
        std::string entryName = entry->d_name;

        // Skip current and parent directory entries
        if (entryName != "." && entryName != "..") {
            // Create links for each entry
            outputFile << "<li><a href=\"" << entryName << "\">" << entryName << "</a></li>\n";
        }
    }

    // Write HTML footer
    outputFile << "</ul></body></html>\n";

    outputFile.close();
    closedir(dir);

    std::cout << "Autoindexing completed. Output file: " << outputFileName << std::endl;
}

std::string getFileExtension(const std::string& filePath) {
    std::string::size_type dotPos = filePath.rfind('.');

    if (dotPos != std::string::npos) {
        return filePath.substr(dotPos + 1);
    }

    return "";
}

bool client::isCGI(const std::string& filePath, server& _server){
	std::string exe = getFileExtension(filePath);
	vector< std::pair<string, string> > cgi_exe = _server.getCgiExe();
	for (size_t i = 0; i < cgi_exe.size(); i++)
	{
		if (exe == cgi_exe[i].second)
		{
			requestObj.is_CGI = 1;
			requestObj.cgi_exe = cgi_exe[i];
			return 1;
		}
	}
	return 0;
	
}

void	client::requestCases(request &requestObj, server& _server)
{
	if (requestObj.getMethod() == "GET") {
		if (!fileExists(requestObj.getFilePath().c_str()) && !isDirectory(requestObj.getFilePath().c_str())) {
			requestObj.setStatusCode(404);
			requestObj.setContentType("text/html");
			requestObj.setFilePath(errorPageTamplate("404, Not Found."));
			return ;
		}
		if (isDirectory(requestObj.getFilePath().c_str()))
		{
			if (!endsWithSlash(requestObj.getFilePath()))
			{
				requestObj.setStatusCode(301);
				requestObj.setContentType("text/html");
				requestObj.setFilePath(requestObj.getFilePath() + "/");
				return ;
			}
			if (!_server.getIndex().empty()) {
				if (!_server.getAutoindex()) {
					requestObj.setStatusCode(403);
					requestObj.setContentType("text/html");
					requestObj.setFilePath(errorPageTamplate("403, Forbidden."));
					return ;
				}
				else {
					generateAutoIndex(requestObj.getFilePath(), "autoindex.html");//?need to do lmsa l file d index
					requestObj.setStatusCode(200);
					requestObj.setContentType("text/html");
					requestObj.setFilePath("autoindex.html");
					return ;
				}
			}
			else {
				if (isCGI(requestObj.getFilePath(), _server)) {
						cout << RED << "CGI_GET" << RESET_TEXT << endl;
					Cgi	cgi(_server, requestObj.loc, requestObj);
					cgi.exe();
					requestObj.setCgiBody(cgi.body);
					requestObj.setCgiHeader(cgi.header);
				}
			}
		}
		else {
			if (isCGI(requestObj.getFilePath(), _server)) {
				cout << RED << "CGI_GET" << RESET_TEXT << endl;
				Cgi	cgi(_server, requestObj.loc, requestObj);
				cgi.exe();
				requestObj.setCgiBody(cgi.body);
				requestObj.setCgiHeader(cgi.header);
			}
		}
//!if uri in get has "?" take until ?
	}
	else if (requestObj.getMethod() == "POST") {
		if (_server.getUpload()) {
			requestObj.parseRequest(requestObj.theBody, _server);
			cout << RED << "UPLOAD IS ON" << RESET_TEXT << endl;
		}
		if (!_server.getUpload())
				cout << RED << "UPLOAD IS OFF" << RESET_TEXT << endl;
		if (!fileExists(requestObj.getFilePath().c_str()) && !isDirectory(requestObj.getFilePath().c_str())) {
			requestObj.setStatusCode(404);
			requestObj.setContentType("text/html");
			requestObj.setFilePath(errorPageTamplate("404, Not Found."));
			return ;
		}
		if (isDirectory(requestObj.getFilePath().c_str()))
		{
			if (!endsWithSlash(requestObj.getFilePath()))
			{
				requestObj.setStatusCode(301);
				requestObj.setContentType("text/html");
				requestObj.setFilePath(requestObj.getFilePath() + "/");
				return ;
			}
			else {
				if (!_server.getIndex().empty()) {
					requestObj.setStatusCode(403);
					requestObj.setContentType("text/html");
					requestObj.setFilePath(errorPageTamplate("403, Forbidden."));
					return ;
				}
				else {
					if (isCGI(requestObj.getFilePath(), _server)) {
						cout << RED << "CGI_POST" << RESET_TEXT << endl;
						Cgi	cgi(_server, requestObj.loc, requestObj);
						cgi.exe();
						requestObj.setCgiBody(cgi.body);
						requestObj.setCgiHeader(cgi.header);
					}
				}
			}
		}
		else {
			if (isCGI(requestObj.getFilePath(), _server)){
				cout << RED << "CGI_post" << RESET_TEXT << endl;
				Cgi	cgi(_server, requestObj.loc, requestObj);
				cgi.exe();
				requestObj.setCgiBody(cgi.body);
				requestObj.setCgiHeader(cgi.header);
			}
		}
	}
	else if (requestObj.getMethod() == "DELETE") {

	}
}

void	client::set_request(string r, server& _server){
	if (!requestObj.headersDone) {
		if (!requestObj.getHeadersRequest(r)) {
			cout <<RED<< "ERROR1" <<RESET_TEXT << endl;
		}
	}
	if (requestObj.headersDone == 1) {
		if (requestObj.checkRequestLine(r)) {
			cout <<RED<< "ERROR2" <<RESET_TEXT << endl;
		}
		else
			requestObj.headersDone = 2;
	}
	if (requestObj.headersDone == 2) {
		if (requestObj.checkHeaderFields(r.substr(0, r.find("\r\n\r\n")))) {
			cout <<RED<< "ERROR3" <<RESET_TEXT << endl;
		}
		else
			requestObj.headersDone = 3;
	}
	requestObj.setContentLength();//!
	requestObj.setContentType();
	if (requestObj.getMethod() == "POST")
		tookrequest = requestObj.getBodyRequest(r);
	if (requestObj.getMethod() == "GET")//!add delete
		tookrequest = 1;
	
    if (tookrequest == 1) {
		
		requestObj.matchLocation(_server);
		requestCases(requestObj, _server);
		responseObj.totalSent = 0;
		responseObj.initialize(requestObj);
	}
}

void	client::set_response(int connection_socket){
	if (!responseObj.totalSent)
		responseObj.sendHeader(connection_socket, requestObj);
	filesent = responseObj.sendBody(connection_socket);
	if (filesent == 1)
	    tookrequest = 0;
}

string client::getresponse(){
    return responsestring;
}

bool	client::getfilesent(){
    return filesent;
}

bool	client::getTookrequest(){
    return tookrequest;
}