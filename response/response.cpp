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
    int bytes_sent;
    if (uri == "/")
        uri = "/public/index.html";
    else if (uri == "/favicon.ico")
        return "";

    std::ifstream File(uri.substr(1), std::ios::binary);

    if (!File.is_open()){
        cout << RED << "Error oppening " << uri << RESET_TEXT << endl;
        return "";
    }
    int fd = open(uri.substr(1).c_str(), O_RDONLY);
   struct stat stat_source;
    if (fstat(fd, &stat_source) == -1) {
        perror("Error getting file status");
        close(fd);
        return "";
    }

    char buffer[stat_source.st_size];
    File.read(buffer, stat_source.st_size);

	std::string header;
	if (uri == "/public/index.html")
	    contentType = "text/html";
	else
	    contentType = "image/png";

	cout << MAGENTA << " type: " << contentType <<
	    " , contnLEN: " << stat_source.st_size << " , uri: " << uri << endl;
	header = "HTTP/1.1 200 OK\r\n"
		"Content-Length: " + std::to_string(stat_source.st_size) + "\r\n"
		"Content-Type: "+ contentType + "\r\n\r\n";
	cout << YELLOW << "SENDING RESPONSE 1 ..." << RESET_TEXT << endl;
	
	bytes_sent = send(connection_socket,
	    header.c_str(), strlen(header.c_str()), 0);//response
	cout << bytes_sent << '/' << strlen(header.c_str()) << " sent" << endl;
	
	cout << YELLOW << "SENDING RESPONSE 2 ..." << RESET_TEXT << endl;
	bytes_sent = send(connection_socket,
	    buffer, stat_source.st_size, 0);//response
	cout << bytes_sent << '/' << stat_source.st_size << " sent" << endl;
    
    cout << YELLOW << header << RESET_TEXT << endl;

	return header + string(buffer);
}

void	response::set_res(int connection_socket){
	res = ReesponseBody(uri, contentType, connection_socket);
}

string    response::get_res(){
    return res;
}