#include "../server.hpp"
using std::cout;
using std::endl;
using std::string;
using std::vector;

int checkExistance(request& requestObj){
    string path;
    if (requestObj.getMethod() == "DELETE")
        path = requestObj.loc.getRoot() + requestObj.getrequestURI();
	else
        path = requestObj.getFilePath();
    if (!fileExists(path.c_str())) {
		requestObj.setStatusCode("404 not found");
		requestObj.setContentType("text/html");
		requestObj.setFilePath(errorPageTamplate("404, Not Found."));
		return 1;
	}
	return 0;
}

bool endsWithSlash(const std::string& str) {
    if (str.length() == 0)
        return false;

    char lastChar = str[str.length() - 1];
    return lastChar == '/';
}

std::string getFileExtension(const std::string& filePath) {
    std::string::size_type dotPos = filePath.rfind('.');

    if (dotPos != std::string::npos) {
        return filePath.substr(dotPos + 1);
    }

    return "";
}

void	isCGI(request& requestObj, const std::string& filePath,
	server& _server){
	std::string exe = getFileExtension(filePath);
	vector< std::pair<string, string> > cgi_exe = _server.getCgiExe();
	for (size_t i = 0; i < cgi_exe.size(); i++)
	{
		if (exe == cgi_exe[i].second)
		{
			requestObj.is_CGI = 1;
			requestObj.cgi_exe = cgi_exe[i];
            requestObj.CgiObj = new Cgi(&_server, &requestObj);
			requestObj.CgiObj->exe();
            requestObj.Cgisdone = 1;
			break;
		}
	}
}

void generateAutoIndex(const std::string& directoryPath, const std::string& outputFileName) {
    DIR* dir = opendir(directoryPath.c_str());

    if (!dir) {
        std::cerr << "Error opening directory: " << strerror(errno) << std::endl;
        return;
    }
	if (fileExists(outputFileName.c_str()))
		unlink(outputFileName.c_str());
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

void	codeNpath(request& requestObj, const char* statusCode, const char* filePath){
	requestObj.setStatusCode(statusCode);
	requestObj.setContentType("text/html");
	requestObj.setFilePath(filePath);
    requestObj.setmethod("GET");
    requestObj.is_CGI = 0;
}

void internalServerError(int connection_socket){
	std::string __response = "HTTP/1.1 500 Internal Server Error\r\n\r\n"
					"Content-Length: 809\n"
					"Content-Type: text/html\n"
		 "<!DOCTYPE html>\n"
		 "<html>\n"
			 "<head>\n"
				 "<style>\n"
					 "html, body {font-family: 'Roboto Mono', monospace;font-size: 16px;}\n"
					 "body {background-color: black;margin: 0;padding: 0;}\n"
					 "p {color: white;font-size: 25px;letter-spacing: 0.2px;margin: 0;display: inline;}\n"
					 ".center-xy {text-align: center;top: 50%;left: 50%;transform: translate(-50%, -50%);position: absolute;}\n"
				 "</style>\n"
			 "</head>\n"
			 "<body>\n"
				 "<div class='center-xy'>\n"
					 "<p id='myP'>\n"
						 "500 Internal Server Error\n"
					 "</p>\n"
				 "</div>\n"
				 "<script>\n"

				
			"let divElement = document.getElementById(\"myP\");"
			"let textContent = divElement.innerText.toString();"
			"let i = 1;"
			"function typeWriter() {"
			"divElement.innerText = textContent.slice(0, i);"
			"console.log(divElement.innerText);"
			"i++;"
			"if (i <= textContent.length)"
			"setTimeout(typeWriter, 100);"
			"}"
			"setTimeout(typeWriter, 0);\n"
				 "</script>\n"
			 "</body>\n"
		 "</html>\n";
	write(connection_socket, __response.c_str(),    //header
		strlen(__response.c_str()));
}