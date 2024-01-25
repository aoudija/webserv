#include "../server.hpp"
using std::cout;
using std::endl;
using std::string;
using std::vector;

void	_get_(request& requestObj,server& _server){
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
		if (!_server.getIndex().empty()) {
			if (!_server.getAutoindex()) {
				codeNpath(requestObj, "403 Forbidden", "text/html");
				return ;
			}
			else {
				generateAutoIndex(requestObj.getFilePath(), "autoindex.html");//?need to do lmsa l file d index
				codeNpath(requestObj,"200 ok", "autoindex.html");
				return ;
			}
		}
		else 
				isCGI(requestObj, requestObj.getFilePath(), _server);
	}
	else 
		isCGI(requestObj, requestObj.getFilePath(), _server);
}
