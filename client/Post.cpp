#include "../server.hpp"
using std::cout;
using std::endl;
using std::string;
using std::vector;

void	_post_(request& requestObj, server& _server){
	if (_server.getUpload()) {
		requestObj.setStatusCode("201 Created");
		requestObj.parseRequest(requestObj.theBody, _server);
	}
	if (!_server.getUpload()){
		if (checkExistance(requestObj))
			return ;
		if (isDirectory(requestObj.getFilePath().c_str()))
		{
			if (!endsWithSlash(requestObj.getFilePath()))
			{
				codeNpath(requestObj,"301 Moved Permanently",
					string(requestObj.getFilePath() + "/").c_str());
				return ;
			}
			else {
				if (!_server.getIndex().empty()) {
					codeNpath(requestObj, "403 Forbidden", errorPageTamplate("403, Forbidden.").c_str());
					return ;
				}
				else 
					isCGI(requestObj, requestObj.getFilePath(), _server);
			}
		}
		else 
			isCGI(requestObj, requestObj.getFilePath(), _server);
	}
}
