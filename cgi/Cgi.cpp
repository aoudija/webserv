#include "Cgi.hpp"
#include <sys/time.h>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


Cgi::Cgi(server &serv, Location &location, request &req): MyServer(serv), MyLocation(location), MyRequest(req){
	this->env = NULL;
}

Cgi::~Cgi()
{
	int i = -1;
	while(this->env[++i])
		delete this->env[i];
	delete [] this->env;
	i = -1;
	while (this->arg[++i])
		delete this->arg[i];
	delete [] this->arg;
}

char* strjoin(const std::string& s1, const std::string& s2) {
    size_t len = s1.length() + s2.length();
    char* result = new char[len + 1];
    strcpy(result, s1.c_str());
    strcat(result, s2.c_str());
    return result;
}


std::string toUpperCase(const std::string& input) {
    std::string result = input;
    
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = std::toupper(result[i]);
    }

    return result;
}

void replace_(std::string &input) {
    size_t pos = input.find('-');
    while (pos != std::string::npos) {
        input.replace(pos, 1, "_");
        pos = input.find('-', pos + 1);
    }
}


std::string getroot(const std::string& filepath) {
    // Find the position of the last directory separator
    size_t lastSeparator = filepath.find_last_of("/\\");

    // If a separator is found, extract the substring up to that position
    if (lastSeparator != std::string::npos)
        return filepath.substr(0, lastSeparator);
	else
        return filepath;
        // If no separator is found, assume the root is the entire path
}

void	Cgi::init(){
	(void)MyLocation;
	std::map<string, string> mapenv;
	std::map<string, string> headerenv = MyRequest.headerFields;
	std::map<string, string>::iterator it = headerenv.begin();
	for(; it != headerenv.end(); it++){
		// std::cout << RED << it->first << "=" <<it->second << RESET_TEXT << std::endl;
		string key = toUpperCase(it->first);
		replace_(key);
		string value = it->second;
		mapenv[key] = value;
	}
	mapenv["SERVER_PROTOCOL"] = "HTTP/1.1";//
	mapenv["GATEWAY_INTERFACE"] = "CGI/1.1";//
	mapenv["SERVER_SOFTWARE"] = "webserv/1.0";//
	mapenv["REDIRECT_STATUS" ] = "0";//
	mapenv["SERVER_NAME"] = this->MyServer.getServerName();//
	mapenv["SERVER_PORT"] = this->MyServer.getPort();//
	mapenv["PATH_INFO"] = this->MyRequest.getFilePath();//
	mapenv["PATH_TRANSLATED"] = this->MyRequest.getFilePath();
	mapenv["SCRIPT_NAME"] = this->MyRequest.getFilePath();
	mapenv["DOCUMENT_ROOT"] = getroot(this->MyRequest.getFilePath());
	mapenv["HTTP_HOST"] = this->MyServer.get_ip();
	mapenv["HTTP_CONNECTION"] = this->MyRequest.headerFields["Connection"];
	mapenv["HTTP_ACCEPT"] = this->MyRequest.headerFields["Accept"];
	mapenv["HTTP_USER-AGENT"] = this->MyRequest.headerFields["User-Agent"];
	mapenv["HTTP_COOKIE"] = this->MyRequest.headerFields["Cookie"];
	mapenv["REQUEST_URI"] = this->MyRequest.getFilePath();;
	if (this->MyRequest.getMethod() == "POST"){
		mapenv["CONTENT_TYPE"] = this->MyRequest.headerFields["Content-Type"];//clien content type
		mapenv["CONTENT_LENGTH"] = this->MyRequest.headerFields["Content-Length"];//clien content lengh
		mapenv["REQUEST_METHOD"] = this->MyRequest.getMethod();
		mapenv["QUERY_STRING"] = MyRequest.theBody;
	}
	else if (this->MyRequest.getMethod() == "GET"){
		mapenv["QUERY_STRING"] = this->MyRequest.getQueryString();
		mapenv["CONTENT_TYPE"] = this->MyRequest.headerFields["Content-Type"];
		mapenv["CONTENT_LENGTH"] = "";
		mapenv["REQUEST_METHOD"] = this->MyRequest.getMethod();
	}
	std::map<string, string>::iterator it2 = mapenv.begin();
	for(; it2 != mapenv.end(); it2++){
		string key = toUpperCase(it2->first);
		string value = it2->second;
			// std::cout << RED << key << "=" <<value << RESET_TEXT << std::endl;
	}
	size_t len = mapenv.size();
	this->env = new char*[len+1];
	size_t i = 0;
	 for (std::map<std::string, std::string>::const_iterator it = mapenv.begin(); it != mapenv.end(); ++it) {
        // Concatenate "key+value"
        // std::string keyValue = it->first + "+" + it->second;

        // Allocate memory for the string and copy the content
        this->env[i] = strjoin(it->first + "=", it->second);
        // std::strcpy(this->env[i], keyValue.c_str());

        ++i;
    }
	
	this->env[len] = NULL;

}

void	Cgi::set_arg(){
	this->arg = new char*[3];
	arg[0] = strdup(MyRequest.cgi_exe.first.c_str());
	arg[1] = strdup(this->MyRequest.getFilePath().c_str());
	arg[2] = NULL;
}

std::string generateFileName() {
    struct timeval	tp;
	long			time;

	gettimeofday(&tp, NULL);
	time = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    std::stringstream ss;
    ss << time;

    // Generate t'he file name using the current date and time
    std::string fileName = "/tmp/file_" + ss.str();
    std::ifstream file(fileName.c_str());
    if (file.good()) {
        fileName = generateFileName();
    }
    return fileName;
}

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");

    if (start == std::string::npos || end == std::string::npos) {
        return "";  // String is all whitespaces
    }
    return str.substr(start, end - start + 1);
}

std::string RandomString()
{
    char alpha[26] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g',
                          'h', 'i', 'j', 'k', 'l', 'm', 'n',
                          'o', 'p', 'q', 'r', 's', 't', 'u',
                          'v', 'w', 'x', 'y', 'z' };
    std::string result = "";
    for (int i = 0; i<26; i++)
        result = result + alpha[rand() % 26];
    return result;
}

void	Cgi::parseHeader(std::vector<std::string> header, size_t len){
	std::stringstream ss;
	ss << len;
	string head;
	string	status;
	string	type;
	string	location;
	if (!header.empty()){
		 for (size_t i = 0; i < header.size(); ++i) {

			if(header[i].find("Status:") != std::string::npos){
				status = "HTTP/1.1 " + trim(header[i].substr(7)) + "\r\n";
			}
			else if(header[i].find("Content-type:") != std::string::npos){
				type = "Content-type: text/html; charset=UTF-8\r\n";
			}
			else if(header[i].find("Location:") != std::string::npos){
				location = header[i] + "\r\n";
			}
			else
				head +=  header[i] + "\r\n";
		}
	}
		head += "Content-Length: " + ss.str() + "\r\n";
	head = location + head;
	if (status.empty())
		head = "HTTP/1.1 200 OK\r\n" + head;
	else
		head = status + head;
	this->header = head + "\r\n\0";
	std::cout << MAGENTA << head << RESET_TEXT << std::endl;
}




std::vector<std::string> splitString(const std::string& input) {
    std::vector<std::string> result;
    std::istringstream iss(input);

    while (!iss.eof()) {
        std::string line;
        std::getline(iss, line, '\n');
		line = trim(line);
        if (!line.empty()) {
            result.push_back(line);
        }
    }
    return result;
}

void handleTimeout(int sig)
{
	(void)sig;
	exit(500);
}

void Cgi::exe(){
		// std::cout << BLUE << "dklsfkljdsklfjkldsjfkjdskljfkldjs" << std::endl;
	init();
	set_arg();
	
	pid_t	pid;
	std::string filename= "0";
	std::string filename2= "1";
	int fileDescriptor1 = open(filename2.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);

	write(fileDescriptor1, MyRequest.theBody.c_str(), MyRequest.theBody.length());
	close(fileDescriptor1);
	pid = fork();
	// std::cerr << "1" << filename << std::endl;
	if (!pid) {
		int	fd = open(filename.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0666);
		dup2(fd, 1); // need protection
		close(fd);
		if (MyRequest.getMethod() == "POST"){
			std::cerr << filename2 << std::endl;
			int fd1 = open(filename2.c_str(), O_RDONLY);
			dup2(fd1, 0);
			close(fd1);
		}
		signal(SIGALRM, handleTimeout);
		alarm(2);
		execve(this->arg[0], this->arg, this->env);
		exit(10);
	}
	int status;
    waitpid(pid, &status, 0);
	if (status > 0){
		std::cerr << status << ": errrrooo cgi" << std::endl;
		exit(0);
	}


	std::string body;
	std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }
    std::ostringstream content;
    content << file.rdbuf();
    file.close();
	body = content.str();

	std::vector<std::string> result;

	std::string header;
	bool	is_header = 0;
	if (body.find("\r\n\r\n") != std::string::npos){
		is_header = 1;
		header = body.substr(0,body.find("\r\n\r\n") + 4);
		body = body.substr(body.find("\r\n\r\n") + 4);
		result = splitString(header);
	}
	body = trim(body);
	parseHeader(result, body.size());


	this->body = body;

	std::string filename3 = generateFileName()+"3";
	int fileDescriptor = open(filename3.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);

	write(fileDescriptor, body.c_str(), body.length());
	close(fileDescriptor);
	this->body = filename3;
}
