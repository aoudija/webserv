#include "../server.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
//

response::response()
{
	firstT = 0;
	totalSent = 0;
	filesize = 0;
	header = "";
	resTime = -1;
	buffer = NULL;
}

void	response::initialize(request& request){
	int fd;
		cout << RED<< "file path is: " << request.getFilePath() << RESET_TEXT << endl;
	if (request.is_CGI)
		fd = open(request.getCgiBody().c_str(), O_RDONLY);
	else
		fd = open(request.getFilePath().c_str(), O_RDONLY);
	filesize = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	if (buffer != NULL) {
		delete buffer;
		buffer = NULL;
	}
	buffer = new char[filesize];
	int c = read(fd, buffer, filesize);
	if (c <= 0)
		throw(std::runtime_error("read error"));
	cout <<"read file int: " << c << endl;
	close(fd);
}

int	response::sendHeader(int connection_socket, request& request){
	if (!request.getredirectURL().empty()){
		header = "HTTP/1.1 301 Moved Permanently\r\n"
			"Location: "+ request.getredirectURL() + "\r\n\r\n"+'\0';
		int bytes_sent = write(connection_socket, header.c_str(),    //header
		strlen(header.c_str()));
		if (bytes_sent <= 0){
			perror("write");//!remove fd
			return 0;
		}
		cout << header <<endl;
		cout << bytes_sent << endl;
		resTime = time(0);
		return 1;
	}
	else if (request.getMethod() == "DELETE")
	{
		header = "HTTP/1.1 " + request.getStatusCode()+ "\r\n\r\n"+'\0';
		int bytes_sent = write(connection_socket, header.c_str(),    //header
		strlen(header.c_str()));
		if (bytes_sent <= 0){
			perror("write");//!remove fd
			return 0;
		}
		resTime = time(0);
		return 1;
	}
	if (request.is_CGI){
		header = request.getCgiHeader();
	}
	else {
		cout << "content type is: "<< BLUE << request.getContentType() << RESET_TEXT << endl;
		header = "HTTP/1.1 " + request.getStatusCode()+ "\r\n"
			"Content-Length: " + std::to_string(filesize) + "\r\n"
			"Content-Type: "+ request.getContentType() + "\r\n\r\n"+'\0';
		cout<<RED<<"statuscode: " << request.getStatusCode() << RESET_TEXT<<endl;
	}
	return 1;
}

int	response::sendBody(int connection_socket){
	if (!firstT){
		firstT++;
		int bytes_sent = write(connection_socket, header.c_str(),//header
		strlen(header.c_str()));
		if (bytes_sent <= 0){
			perror("write");//!remove fd
			return -1;
		}
		resTime = time(0);
		return 0;
	}
	int bytes_sent;
	size_t len = 1024;
	if (len > filesize - totalSent)
		len = filesize - totalSent;
	bytes_sent = write(connection_socket, buffer + totalSent, len);
	if (bytes_sent <= 0){
		perror("write");//!remove fd
		return -1;
	}
	resTime = time(0);
	totalSent += bytes_sent;
	if (totalSent >= filesize){
		totalSent = 0;
		firstT = 0;
		delete buffer;
		buffer = NULL;
		return 1;
	}
	return 0;
}

void	response::reset(){
	firstT = 0;
	totalSent = 0;
	filesize = 0;
	header = "";
	if (buffer)
		delete buffer;
	buffer = NULL;
}
