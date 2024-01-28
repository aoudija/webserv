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

bool	isCGI(request& requestObj, const std::string& filePath,
	server& _server){
	std::string exe = getFileExtension(filePath);
	vector< std::pair<string, string> > cgi_exe = _server.getCgiExe();
	for (size_t i = 0; i < cgi_exe.size(); i++)
	{
		if (exe == cgi_exe[i].second)
		{
			requestObj.is_CGI = 1;
			requestObj.cgi_exe = cgi_exe[i];
			Cgi	cgi(_server, requestObj.loc, requestObj);
			cgi.exe();
			requestObj.setCgiBody(cgi.body);
			requestObj.setCgiHeader(cgi.header);
			return 1;
		}
	}
	return 0;
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
}
