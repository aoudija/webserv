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
        uri = "/public/get-to-work-work.png";

    std::ifstream File(uri.substr(1));
    if (!File.is_open())
        cout << RED << "Error oppening " << uri << RESET_TEXT << endl;

    string Content,c;
    while (std::getline(File, c))
        Content += c;

    std::string msg;
    if (uri == "/public/index.html")
        contentType = "text/html";
    else
        contentType = "image/png";
    cout << MAGENTA << " type: " << contentType << " , contnLEN: " << Content.size() << " , uri: " << uri << endl;
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