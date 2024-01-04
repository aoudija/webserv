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

void	response::set_res(int connection_socket, request& request){
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
		return ;
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
	bytes_sent = send(connection_socket,
		buffer, filesize, 0);//header
	// size_t total;
	// total = 0;
	// bytes_sent = 0;
	// while (total < filesize){
		// bytes_sent = write(connection_socket, buffer + total, filesize - total);
		// while (bytes_sent < 0){
		// 	bytes_sent = write(connection_socket, buffer + total, filesize - total);
		// }
		// total += bytes_sent;
	// }
		// cout <<WHITE << "this is total bytes: " << total << RESET_TEXT << endl;
	offset = bytes_sent;
	free(buffer);
	close(fd);
	cout << YELLOW << "BODY RESPONSE SENT..." << RESET_TEXT << endl;
	cout << GREEN << bytes_sent << '/' << filesize << " sent" << endl;
	res = "";
}

string    response::get_res(){
    return res;
}
//SEND():
// FAILS with ->> 351608 Bytes left on socket buffer .
// WORKS FINE with ->> 638328 Bytes left on socket buffer .