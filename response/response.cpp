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
	buffer = new char[filesize];
	int c = read(fd, buffer, filesize);
	cout <<"read file int: " << c << endl;
	close(fd);
}

void	response::sendHeader(int connection_socket, request& request){
	std::string header;
	if (!request.getredirectURL().empty()){
		header = "HTTP/1.1 301 Moved Permanently\r\n"
			"Location: "+ request.getredirectURL() +'\0';
	}
	else {
		cout << "content type is: "<< request.getContentType() << endl;
		header = "HTTP/1.1 " + request.getStatusCode()+ "\r\n"
			"Content-Length: " + std::to_string(filesize) + "\r\n"
			"Content-Type: "+ request.getContentType() + "\r\n\r\n"+'\0';
		cout<<RED<<"statuscode: " << request.getStatusCode() << RESET_TEXT<<endl;
	}
	if (request.is_CGI){
		write(connection_socket, request.getCgiHeader().c_str(),
			strlen(request.getCgiHeader().c_str()));
	}
	else
		write(connection_socket, header.c_str(),	//header
		strlen(header.c_str()));
}

int	response::sendBody(int connection_socket){
	size_t len = 1024;
	if (len > filesize - totalSent)
		len = filesize - totalSent;
	int bytes_sent = write(connection_socket, buffer + totalSent, len);
	if (bytes_sent <= 0){
		perror("write");
		totalSent++;
	}
	totalSent += bytes_sent;
	int allFileSent = 0;
	if (totalSent >= filesize){
		allFileSent = 1;
		totalSent = 0;
		free(buffer);
	}
	return allFileSent;
}
