#include "../server.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
//

void	response::setURI(string uri){
	this->uri = uri;
}
void	response::setcontentType(string contenttype){
	contentType = contenttype;
}

string ReesponseHeaderNBody(request& request, int connection_socket) {//this is get
	fcntl(connection_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	int bytes_sent;
	string uri = request.getrequestURI();
	string contentType = request.getContentType();
	// int startFrom = request.getBytesRange();
	if (uri == "/")//till we get the LOCATIONS 
		uri = "/public/index.html";
	std::ifstream File(uri.substr(1));
	if (!File.is_open()){
		cout << RED << "Error oppening " << uri << RESET_TEXT << endl;
		return "";
	}
	int fd = open(uri.substr(1).c_str(), O_RDONLY);
	size_t filesize = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	char* buffer = (char*)malloc(filesize + 1);
	read(fd, buffer, filesize);
	std::string header = "HTTP/1.1 200 OK\r\n"
		"Content-Length: " + std::to_string(filesize) + "\r\n"
		"Content-Type: "+ contentType + "\r\n\r\n";
	bytes_sent = send(connection_socket,
		header.c_str(), strlen(header.c_str()), 0);//header
	// size_t totalBytes = 0;
	// int bufferSize = filesize;  // Set your desired buffer size
	// if(setsockopt(connection_socket, SOL_SOCKET, SO_SNDBUF,
	// 	&bufferSize, sizeof(bufferSize)))
	// 	perror("Setsockopt failed");
    int sendBufferSize;
    socklen_t optlen = sizeof(sendBufferSize);
	size_t total;
	total = bytes_sent = 0;
	while (total < filesize){
		if (getsockopt(connection_socket, SOL_SOCKET, SO_SNDBUF,
			&sendBufferSize, &optlen) == -1){
			perror("Getsockopt failed");
			exit(EXIT_FAILURE);}
		printf("BEFORE: Send buffer size: %d bytes vs %d\n", sendBufferSize, bytes_sent);
		bytes_sent = send(connection_socket,
			buffer + total, filesize - total, 0);
		while (bytes_sent < 0){
			bytes_sent = send(connection_socket,
				buffer + total, filesize - total, 0);
		}
		total += bytes_sent;
	}
		cout <<WHITE << "this is total bytes: " << total << RESET_TEXT << endl;
	free(buffer);
	close(fd);
	cout << YELLOW << "BODY RESPONSE SENT..." << RESET_TEXT << endl;
	cout << GREEN << total << '/' << filesize << " sent" << endl;
	return "";
}

void	response::set_res(int connection_socket, request& request){
	res = ReesponseHeaderNBody(request, connection_socket);
}

string    response::get_res(){
    return res;
}
//SEND():
// FAILS with ->> 351608 Bytes left on socket buffer .
// WORKS FINE with ->> 638328 Bytes left on socket buffer .