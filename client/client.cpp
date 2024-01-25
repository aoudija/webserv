#include "../server.hpp"
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;


void	client::requestCases(request &requestObj, server& _server)
{
	if (requestObj.getMethod() == "GET") {
		_get_(requestObj,_server);
//!if uri in get has "?" take until ?
	}
	else if (requestObj.getMethod() == "POST") {
		_post_(requestObj,_server);
	}
	else if (requestObj.getMethod() == "DELETE")
		_delete_(requestObj, _server);
}

void	client::set_request(string r, server& _server){
	if (!requestObj.headersDone) {
		if (!requestObj.getHeadersRequest(r)) {
			cout <<RED<< "ERROR1" <<RESET_TEXT << endl;
		}
	}
	if (requestObj.headersDone == 1) {
		if (requestObj.checkRequestLine(r)) {
			cout <<RED<< "ERROR2" <<RESET_TEXT << endl;
		}
		else
			requestObj.headersDone = 2;
	}
	if (requestObj.headersDone == 2) {
		if (requestObj.checkHeaderFields(r.substr(0, r.find("\r\n\r\n")))) {
			cout <<RED<< "ERROR3" <<RESET_TEXT << endl;
		}
		else
			requestObj.headersDone = 3;
	}
	requestObj.setContentLength();//!
	requestObj.setContentType();
	if (requestObj.getMethod() == "POST")
		tookrequest = requestObj.getBodyRequest(r);
	if (requestObj.getMethod() == "GET")//!add delete
		tookrequest = 1;
	
    if (tookrequest == 1) {
		requestObj.matchLocation(_server);
		requestCases(requestObj, _server);
		responseObj.totalSent = 0;
		responseObj.initialize(requestObj);
	}
}

void	client::set_response(int connection_socket){
	if (!responseObj.totalSent)
		responseObj.sendHeader(connection_socket, requestObj);
	if (!requestObj.getredirectURL().empty())
		filesent = 1;
	else
		filesent = responseObj.sendBody(connection_socket);
	if (filesent == 1)
	    tookrequest = 0;
}

string client::getresponse(){
    return responsestring;
}

bool	client::getfilesent(){
    return filesent;
}

bool	client::getTookrequest(){
    return tookrequest;
}