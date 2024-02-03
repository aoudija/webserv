#include "../server.hpp"
using std::cout;
using std::endl;
using std::string;
using std::vector;

void	_post_(request& requestObj, server& _server){
	if (_server.getUpload()) {
		requestObj.setStatusCode("201 Created");
		requestObj.parseRequest(requestObj.theBody, _server);
		if (checkExistance(requestObj))
			return ;
		if (isDirectory(requestObj.getFilePath().c_str()))
		{
			if (!endsWithSlash(requestObj.getFilePath()))
			{
				if (!_server.getAutoindex()) {
					codeNpath(requestObj, "403 Forbidden", errorPageTamplate("403, Forbidden.").c_str());
					return ;
				}
				else {
					requestObj.setredirectURL(requestObj.getrequestURI() + "/");
					cout << BLUE << requestObj.getrequestURI() + "/" << RESET_TEXT << endl;
					return ;
				}
			}
			else {
				if (_server.getIndex().empty()) {
					codeNpath(requestObj, "403 Forbidden", errorPageTamplate("403, Forbidden.").c_str());
					return ;
				}
				else{
					isCGI(requestObj, requestObj.getFilePath(), _server);
				}
			}
		}
		else{
			isCGI(requestObj, requestObj.getFilePath(), _server);
		}
	}
	else if (!_server.getUpload()){
		if (checkExistance(requestObj))
			return ;
		if (isDirectory(requestObj.getFilePath().c_str()))
		{
			if (!endsWithSlash(requestObj.getFilePath()))
			{
				requestObj.setredirectURL(requestObj.getrequestURI() + "/");
				return ;
			}
			else {
				if (!_server.getIndex().empty()) {
					codeNpath(requestObj, "403 Forbidden", errorPageTamplate("403, Forbidden.").c_str());
					return ;
				}
				else {
					isCGI(requestObj, requestObj.getFilePath(), _server);
					if (requestObj.is_CGI == 0){
						codeNpath(requestObj, "403 Forbidden", errorPageTamplate("403, Forbidden.").c_str());
						return ;
					}
				}
			}
		}
		else {
			isCGI(requestObj, requestObj.getFilePath(), _server);
			if (requestObj.is_CGI == 0) {
				codeNpath(requestObj, "403 Forbidden", errorPageTamplate("403, Forbidden.").c_str());
				return ;
			}
		}
	}
}
