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

string ReesponseHeaderNBody(request& request, int connection_socket) {
	fcntl(connection_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	//this is get
	int bytes_sent;
	string uri = request.getrequestURI();
	string contentType = request.getContentType();
	int startFrom = request.getBytesRange();
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
	char* buffer = (char*)malloc(filesize);
	read(fd, buffer, filesize);
	std::string header = "HTTP/1.1 200 OK\r\n"
		"Content-Length: " + std::to_string(filesize - startFrom) + "\r\n"
		"Content-Type: "+ contentType + "\r\n\r\n";
	bytes_sent = send(connection_socket,
	    header.c_str(), strlen(header.c_str()), 0);//header
	bytes_sent = send(connection_socket,
	    buffer + startFrom, filesize - startFrom, 0);//body
	free(buffer);
	cout << YELLOW << "BODY RESPONSE SENT..." << RESET_TEXT << endl;
	cout << bytes_sent << '/' << filesize << " sent" << endl;
	return "";
}

void	response::set_res(int connection_socket, request& request){
	res = ReesponseHeaderNBody(request, connection_socket);
}

string    response::get_res(){
    return res;
}