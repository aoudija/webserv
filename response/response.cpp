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

string ReesponseBody(string uri, string contentType, int connection_socket) {
	// fcntl(connection_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	int bytes_sent;
	if (uri == "/")
	    uri = "/public/index.html";
	else if (uri == "/favicon.ico")
	    return "";
	std::ifstream File(uri.substr(1));

	if (!File.is_open()){
	    cout << RED << "Error oppening " << uri << RESET_TEXT << endl;
	    return "";
	}

	int fd = open(uri.substr(1).c_str(), O_RDONLY);
	size_t filesize = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
		
	cout << RED << uri << RESET_TEXT << endl;
	char* buffer = (char*)malloc(filesize);
	File.read(buffer, filesize);

	std::string header;
	if (uri == "/public/index.html")
	    contentType = "text/html";
	else if (uri == "/public/demo_video(1080p).mp4")
	    contentType = "video/mp4";
	else{
	    contentType = "image/png";
		cout << "uri in that case = " << uri << endl;
	}

	cout << MAGENTA << " type: " << contentType <<
	    " , contnLEN: " << filesize << " , uri: " << uri << endl;
	header = "HTTP/1.1 200 OK\r\n"
		"Content-Length: " + std::to_string(filesize) + "\r\n"
		"Content-Type: "+ contentType + "\r\n\r\n";

	bytes_sent = send(connection_socket,
	    header.c_str(), strlen(header.c_str()), 0);//response
	cout<< GREEN << bytes_sent << '/' << strlen(header.c_str()) << " sent" << endl;
    cout << YELLOW << header << RESET_TEXT << endl;
	cout << YELLOW << "HEADER RESPONSE SENT ..." << RESET_TEXT << endl;


	bytes_sent = send(connection_socket,
	    buffer, filesize, 0);//response
	cout << RED << "wa3la qwadaaa" << endl;
	free(buffer);
	cout << YELLOW << "BODY RESPONSE SENT..." << RESET_TEXT << endl;
	cout << bytes_sent << '/' << filesize << " sent" << endl;
	return "";
}

void	response::set_res(int connection_socket){
	res = ReesponseBody(uri, contentType, connection_socket);
}

string    response::get_res(){
    return res;
}