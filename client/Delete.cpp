#include "../server.hpp"
using std::cout;
using std::endl;
using std::string;
using std::vector;


void	_delete_(request& requestObj, server& _server){
	if (checkExistance(requestObj))
			return ;
	(void)_server;
}
