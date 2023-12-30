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

string ReesponseBody(string uri, string contentType) {
    
    if (uri == "/")
        uri = "/public/index.html";
    else if (uri == "/favicon.ico")
        return "";

    std::ifstream File(uri.substr(1), std::ios::binary);
    if (!File.is_open())
        cout << RED << "Error oppening " << uri << RESET_TEXT << endl;

    File.seekg(0, std::ios::end);
    std::streampos filesize = File.tellg();
    File.seekg(0, std::ios::beg);
    char buffer[static_cast<size_t>(filesize)];
    File.read(buffer, filesize);

    std::string msg;
    if (uri == "/public/index.html")
        contentType = "text/html";
    else
        contentType = "image/png";
    cout << MAGENTA << " type: " << contentType <<
        " , contnLEN: " << (size_t)filesize << " , uri: " << uri << endl;
	msg = "HTTP/1.1 200 OK\r\n"
		"Content-Length: " + std::to_string((size_t)filesize) + "\r\n"
		"Content-Type: "+ contentType + "\r\n\r\n" + buffer;
    
    cout << RED << contentType << RESET_TEXT << endl;
    cout << RED << "buffer ----> " << buffer << RESET_TEXT << endl;
    cout << RED << "len buffer ----> " << (size_t)filesize << RESET_TEXT << endl;
	return msg;
}

void	response::set_res(){
	res = ReesponseBody(uri, contentType);
}

string    response::get_res(){
    return res;
}