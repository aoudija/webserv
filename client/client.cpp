#include "../server.hpp"
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;


void	client::requestCases(request &requestObj, server& _server)
{
	if (requestObj.getMethod() == "GET")
		_get_(requestObj,_server);
	else if (requestObj.getMethod() == "POST") 
		_post_(requestObj,_server);
	else if (requestObj.getMethod() == "DELETE")
		_delete_(requestObj);
}

void	client::set_request(string r, server& _server){
	if (!requestObj.headersDone) {
		if (!requestObj.getHeadersRequest(r)) {
			// requestObj.failHeader = true;
			// tookrequest = 1;
			cout <<RED<< "ERROR1" <<RESET_TEXT << endl;
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
		if (requestObj.getMethod() != "DELETE" && requestObj.getredirectURL().empty())
			responseObj.initialize(requestObj);
	}
	else {
		if (requestObj.headersDone == 3 || requestObj.headersDone == 4) {
			requestObj.setContentLength();
			requestObj.setContentType();
			requestObj.setConnection();
			keepAlive = requestObj.getConnection();
			if (requestObj.getMethod() == "POST")
				tookrequest = requestObj.getBodyRequest(r);
			if (requestObj.getMethod() == "GET" || requestObj.getMethod() == "DELETE")
				tookrequest = 1;
				
		}
		if (tookrequest == 1) {
			requestObj.matchLocation(_server);
			requestCases(requestObj, _server);
			responseObj.totalSent = 0;
		}
	}
}

int		client::set_response(int connection_socket){
	if (requestObj.Cgisdone)
	{
		int status = requestObj.CgiObj->waitcheck();
		if (status == 502){
			codeNpath(requestObj,"502 Bad Gateway", errorPageTamplate("502, Bad Gateway").c_str());
			requestObj.Cgisdone = 0;
		}
		else if (status == 504){
			codeNpath(requestObj,"504 Gateway Timeout", errorPageTamplate("504, Gateway Timeout").c_str());
			requestObj.Cgisdone = 0;
		}
		else if (status == 500){
			codeNpath(requestObj,"500 Internal Server Error", errorPageTamplate("500, Internal Server Error").c_str());
			requestObj.Cgisdone = 0;
		}
		else if (status == 1){
			int status2 = requestObj.CgiObj->ParseAll();
			if (status2 == 502){
				codeNpath(requestObj,"502 Bad Gateway", errorPageTamplate("502, Bad Gateway").c_str());
			}
			else{
				requestObj.setCgiBody(requestObj.CgiObj->body);
				requestObj.setCgiHeader(requestObj.CgiObj->header);
			}
			requestObj.Cgisdone = 0;
		}
		if (requestObj.Cgisdone == 0)
			delete requestObj.CgiObj;
	}
	else {
		if (!responseObj.firstT)
		{
			if (requestObj.getMethod() != "DELETE" && requestObj.getredirectURL().empty()){
				responseObj.initialize(requestObj);
			}
			// if (!requestObj.getredirectURL().empty())
			// 	keepAlive = 0;
			if (responseObj.sendHeader(connection_socket, requestObj) == 0)
				return 0;
			resTime = responseObj.resTime;
		}
		if (!requestObj.getredirectURL().empty() || requestObj.getMethod() == "DELETE")
			filesent = 1;
		else{
			filesent = responseObj.sendBody(connection_socket);
			if (filesent == -1)
				return 0;
			resTime = responseObj.resTime;
		}
		if (filesent == 1){
			tookrequest = 0;
			if (requestObj.getFilePath() == "autoindex.html"
				|| requestObj.getFilePath() == "errorpage.html") {
				unlink(requestObj.getFilePath().c_str());
			}
		}
	}
	return 1;
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

void client::reset(){
	responseObj.reset();
	requestObj.reset();
	keepAlive = 0;
	filesent = 0;
	tookrequest = 0;
}