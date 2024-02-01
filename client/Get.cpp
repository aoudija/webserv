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
			cout << MAGENTA << requestObj.getFilePath() << RESET_TEXT << endl;
			if (requestObj.loc.getIndex().empty()) {
				if (!_server.getAutoindex()) {
					codeNpath(requestObj, "403 Forbidden", errorPageTamplate("403, Forbidden.").c_str());
					return ;
				}
			}
			else {
				generateAutoIndex(requestObj.getFilePath(), "autoindex.html");//?need to do lmsa l file d index
				codeNpath(requestObj,"301 Moved Permanently", "autoindex.html");
				return ;
			}
			codeNpath(requestObj, "301 Moved Permanently", (requestObj.getFilePath() + "/" + _server.getIndex()).c_str());
			return ;
		}
		if (_server.getIndex().empty()) {
			if (!_server.getAutoindex()) {
				codeNpath(requestObj, "403 Forbidden", errorPageTamplate("403, Forbidden.").c_str());
				return ;
			}
			else {
				generateAutoIndex(requestObj.getFilePath(), "autoindex.html");//?need to do lmsa l file d index
				codeNpath(requestObj,"200 ok", "autoindex.html");
				return ;
			}
		}
		else {
			isCGI(requestObj, requestObj.getFilePath(), _server);
		}
	}
	else 
		isCGI(requestObj, requestObj.getFilePath(), _server);
}
