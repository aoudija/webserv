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
		_delete_(requestObj);
}

void	client::set_request(string r, server& _server){

	if (!requestObj.headersDone) {
		if (!requestObj.getHeadersRequest(r)) {
			// requestObj.failHeader = true;
			// tookrequest = 1;
			cout <<RED<< "ERROR1" <<RESET_TEXT << endl;//! chi hmaq na9s maybe ??
		}
	}
	if (requestObj.headersDone == 1 || requestObj.failHeader) {
		if (requestObj.checkRequestLine(r)) {
			requestObj.failHeader = true;
			tookrequest = 1;
			cout <<RED<< "ERROR2" <<RESET_TEXT << endl;
		}
		else
			requestObj.headersDone = 2;
	}
	if (requestObj.headersDone == 2 || requestObj.failHeader) {
		if (requestObj.checkHeaderFields(r.substr(0, r.find("\r\n\r\n")))) {
			requestObj.failHeader = true;
			tookrequest = 1;
			cout <<RED<< "ERROR3" <<RESET_TEXT << endl;
		}
		else
			requestObj.headersDone = 3;
	}
	if (requestObj.failHeader && tookrequest) {
		responseObj.totalSent = 0;
		if (requestObj.getMethod() != "DELETE")
			responseObj.initialize(requestObj);
	}
	else {
		if (requestObj.headersDone == 3 || requestObj.headersDone == 4) {
			requestObj.setContentLength();
			requestObj.setContentType();
			if (requestObj.getMethod() == "POST")
				tookrequest = requestObj.getBodyRequest(r);
			if (requestObj.getMethod() == "GET" || requestObj.getMethod() == "DELETE")
				tookrequest = 1;
				
		}
		if (tookrequest == 1) {
			requestObj.matchLocation(_server);
			requestCases(requestObj, _server);
			responseObj.totalSent = 0;
			if (requestObj.getMethod() != "DELETE")
				responseObj.initialize(requestObj);
		}
	}
}

void	client::set_response(int connection_socket){
	if (!responseObj.totalSent)
		responseObj.sendHeader(connection_socket, requestObj);
	if (!requestObj.getredirectURL().empty() || requestObj.getMethod() == "DELETE")
		filesent = 1;
	else
		filesent = responseObj.sendBody(connection_socket);
	if (filesent == 1){
	    tookrequest = 0;
		if (requestObj.getFilePath() == "autoindex.html"
			|| requestObj.getFilePath() == "errorpage.html") {
			unlink(requestObj.getFilePath().c_str());
		}
	}
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