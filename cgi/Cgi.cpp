#include "Cgi.hpp"
#include <sys/time.h>
// Cgi::Cgi(server &serv, Location &location, client &cln): MyServer(&serv), MyLocation(location), MyCient(cln){
// 	this->env = NULL;
// }

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

void	Cgi::init(){
	(void)MyLocation;
	this->env = new char*[17];
	this->env[0] = strjoin("SERVER_PROTOCOL=", "HTTP/1.1");
	this->env[1] = strjoin("GATEWAY_INTERFACE=", "CGI/1.1");
	this->env[2] = strjoin("SERVER_SOFTWARE=", "webserv/1.0");
	this->env[3] = strjoin("REDIRECT_STATUS=" ,"200");
	this->env[4] = strjoin("SERVER_NAME=", this->MyServer.getServerName());
	this->env[5] = strjoin("SERVER_PORT=", this->MyServer.getPort());
	this->env[6] = strjoin("PATH_INFO=", this->MyRequest.getFilePath());
	this->env[8] = strjoin("PATH_TRANSLATED=", this->MyRequest.getFilePath());
	this->env[9] = strjoin("SCRIPT_NAME=", this->MyRequest.getFilePath());
	this->env[10] = strjoin("DOCUMENT_ROOT=", this->MyRequest.getFilePath());
	this->env[12] = strjoin("REMOTE_HOST=", this->MyRequest.headerFields["Host"]);//clien host-name
	this->env[13] = strjoin("REMOTE_ADDR=", this->MyRequest.headerFields["Host"]);//clien IP address
	if (this->MyRequest.getMethod() == "POST"){
		this->env[11] = strjoin("QUERY_STRING=", "");
		this->env[14] = strjoin("CONTENT_TYPE=", this->MyRequest.headerFields["Content-Type"]);//clien content type
		this->env[15] = strjoin("CONTENT_LENGTH=", this->MyRequest.headerFields["Content-Length"]);//clien content lengh
		this->env[7] = strjoin("REQUEST_METHOD=", this->MyRequest.getMethod());
	}
	else if (this->MyRequest.getMethod() == "GET"){
		this->env[11] = strjoin("QUERY_STRING=", this->MyRequest.getQueryString());
		this->env[14] = strjoin("CONTENT_TYPE=", this->MyRequest.headerFields["Content-Type"]);
		this->env[15] = strjoin("CONTENT_LENGTH=", "");
		this->env[7] = strjoin("REQUEST_METHOD=", this->MyRequest.getMethod());
	}
	this->env[16] = NULL;
}

void	Cgi::set_arg(){
	this->arg = new char*[3];
	arg[0] = strdup(MyRequest.cgi_exe.first.c_str());
	arg[1] = strdup(this->MyRequest.getFilePath().c_str());
	arg[2] = NULL;
	// std::cout << "****" << arg[0] << std::endl;
	// std::cout << "****" << arg[1] << std::endl;
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

void	Cgi::parseHeader(std::vector<std::string> header, size_t len){
	std::stringstream ss;
	ss << len;
	string head;
	string	status;
	if (!header.empty()){
		// std::size_t foundPos;
		 for (size_t i = 0; i < header.size(); ++i) {

			if(header[i].find("status:") != std::string::npos){
				status = "HTTP/1.1 " + trim(header[i].substr(7)) + "\r\n";
			}
			else
				head +=  header[i] + "\n";
		}
	}

	if (head.find("Content-type:") == std::string::npos)
		head += "Content-type: text/html; charset=UTF-8\r\n";
		head += "Content-Length: " + ss.str() + "\r\n";
	// std::cout << YELLOW << "**" << ss.str() << RESET_TEXT <<std::endl;
	if (status.empty())
		head = "HTTP/1.1 200 OK\r\n" + head;
	else
		head = status + head;
	this->header = head + "\r\n\0";
	// std::cout << MAGENTA << head << RESET_TEXT << std::endl;
}




std::vector<std::string> splitString(const std::string& input) {
    std::vector<std::string> result;
    std::istringstream iss(input);

    while (!iss.eof()) {
        std::string line;

        // Read until the delimiter "\r\n" or "\n" is found
        std::getline(iss, line, '\n');

        // Append the '\r' character to the substring

        // If there's a '\n' character, append it as well
        // if (iss.peek() == '\n') {
        //     iss.ignore(); // Skip the '\n' character
        // }

		line = trim(line);
        // Skip pushing empty lines
        if (!line.empty()) {
            result.push_back(line);
        }
    }

    return result;
}

void Cgi::exe(){
		// std::cout << BLUE << "dklsfkljdsklfjkldsjfkjdskljfkldjs" << std::endl;
	init();
	set_arg();
	
	pid_t	pid;
	std::string filename= generateFileName()+"0";
	std::string filename2= generateFileName()+"1";
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
			// int a[2];
			// pipe(a);
			int	fd1 = open(filename2.c_str(), O_RDONLY, 0777);
			// if (fd1 == -1)
			// 	std::cerr << " not opeeeenn :" << filename2.c_str() <<std::endl;
			 dup2(fd1, 0);
			close(fd1);
			// write(a[1], "name=data&res=go&hi=belhaj\n", 27);
		}
		execve(this->arg[0], this->arg, this->env);
	}
	waitpid(pid, 0, 0);



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
	// std::cout << BLUE << body << RESET_TEXT << std::endl;
	// std::cout << YELLOW << body.length() << RESET_TEXT << std::endl;
	this->body = body;

	std::string filename3 = generateFileName()+"3";
	int fileDescriptor = open(filename3.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);

	write(fileDescriptor, body.c_str(), body.length());
	close(fileDescriptor);
	this->body = filename3;
}


