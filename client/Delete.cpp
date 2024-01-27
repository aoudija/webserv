#include "../server.hpp"
using std::cout;
using std::endl;
using std::string;
using std::vector;


void	_delete_(request& requestObj){
	requestObj.setStatusCode("204 No Content");
	string path = requestObj.loc.getRoot() + requestObj.getrequestURI();
	if (checkExistance(requestObj))
			return ;
	if (isDirectory(path.c_str())){//directory
		if (!endsWithSlash(path)){//no slash
			cout << "slash makaynash\n";
			codeNpath(requestObj,"409 Conflict",
				errorPageTamplate("409, Conflict.").c_str());
			return ;
		}
		else {//with slash
			DIR* dir = opendir(path.c_str());
			struct dirent* entry;
			vector<const char*> delFiles;
			bool success = true;
			while ((entry = readdir(dir)) != NULL)
			{
				if (strcmp(entry->d_name, ".") != 0
					&& strcmp(entry->d_name, "..") != 0)
				{
					if (entry->d_type == DT_REG){
						if (unlink((path + entry->d_name).c_str()) != 0)
						{
							success = false;
							break;
						}
					}
					else if (entry->d_type == DT_DIR){
						if (rmdir((path + entry->d_name).c_str()) == 0)
						{
							success = false;
							break;
						}
					}
				}
			}
			if (!success)
			{
				if (access(path.c_str(), W_OK) == 0)//has write access
				{
					codeNpath(requestObj,"403 Forbidden",
						errorPageTamplate("403, Forbidden.").c_str());
						return ;
				}
				else
				{//doesnt have write access
					codeNpath(requestObj,"503 Internal Server Error",
						errorPageTamplate("503 , Internal Server Error.").c_str());
						return ;
				}
			}
		}
	}
	else {//is file
		if (unlink(requestObj.getFilePath().c_str()) != 0) {
			codeNpath(requestObj,"403 Forbidden",
			errorPageTamplate("403, Forbidden.").c_str());
			return ;
		}
	}
}
