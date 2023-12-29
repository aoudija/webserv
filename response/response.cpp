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
        uri = "public/index.html";
    std::ifstream File(uri);
    if (!File.is_open())
        cout << RED << "Error oppening " << uri << RESET_TEXT << endl;

    string buffer,c;
    while (std::getline(File, c)){
        buffer += c + "\n";
    }
    std::string Content(buffer);
    std::string msg;
	msg = "HTTP/1.1 200 OK\r\n"
		"Content-Length: " + std::to_string(Content.size()) + "\r\n"
		"Content-Type: "+ contentType + "\r\n\r\n" + Content;
    cout << RED << contentType << RESET_TEXT << endl;
	return msg;
}

void	response::set_res(){
	res = ReesponseBody(uri, contentType);
}

string    response::get_res(){
    return res;
}