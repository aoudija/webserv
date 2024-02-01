#include "../server.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
//

void	response::initialize(request& request){
	int fd;
		cout << RED<< "file path is: " << request.getFilePath() << RESET_TEXT << endl;
	if (request.is_CGI)
		fd = open(request.getCgiBody().c_str(), O_RDONLY);
	else
		fd = open(request.getFilePath().c_str(), O_RDONLY);
	filesize = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	if (buffer != NULL)
	{
		delete buffer;
		buffer = NULL;
	}
	buffer = new char[filesize];
	int c = read(fd, buffer, filesize);
	cout <<"read file int: " << c << endl;
	close(fd);
}

void	response::sendHeader(int connection_socket, request& request){
	if (!request.getredirectURL().empty()){
		header = "HTTP/1.1 301 Moved Permanently\r\n"
			"Location: "+ request.getredirectURL() +'\0';
		write(connection_socket, header.c_str(),    //header
		strlen(header.c_str()));
		return ;
	}
	else if (request.getMethod() == "DELETE")
	{
		header = "HTTP/1.1 " + request.getStatusCode()+ "\r\n\r\n"+'\0';
		write(connection_socket, header.c_str(),    //header
		strlen(header.c_str()));
		return ;
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
}

int	response::sendBody(int connection_socket){
	if (!firstT){
		firstT++;
		int bytes_sent = write(connection_socket, header.c_str(),    //header
		strlen(header.c_str()));
		if (bytes_sent <= 0){
			perror("write");
			totalSent++;
		}
		return 0;
	}
	int bytes_sent;
	size_t len = 1024;
	if (len > filesize - totalSent)
		len = filesize - totalSent;
	bytes_sent = write(connection_socket, buffer + totalSent, len);
	totalSent += bytes_sent;
	int allFileSent = 0;
	if (totalSent >= filesize){
		allFileSent = 1;
		totalSent = 0;
		firstT = 0;
		delete buffer;
		buffer = NULL;
	}
	return allFileSent;
	return 0;
}
