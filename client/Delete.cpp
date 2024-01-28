#include "../server.hpp"
using std::cout;
using std::endl;
using std::string;
using std::vector;

int deleteDirectory(const char* path) {
	DIR* dir = opendir(path);
	if (dir == nullptr) {
		std::cerr << "Error opening directory: " << path << std::endl;
		return 1;
	}

	struct dirent* entry;
	while ((entry = readdir(dir)) != nullptr) {
		if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
			std::string filePath = std::string(path) + "/" + entry->d_name;

			if (entry->d_type == DT_DIR) {
				// Recursively delete subdirectories
				deleteDirectory(filePath.c_str());
				if (rmdir(path) != 0) {
					std::cerr << "Error deleting directory: " << path << std::endl;
					return 1;
				}
			} else {
				// Delete files
				if (unlink(filePath.c_str()) != 0) {
					std::cerr << "Error deleting file: " << filePath << std::endl;
					return 1;
				}
			}
		}
	}

	closedir(dir);

	// Remove the empty directory
	return 0;
}

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
			if (deleteDirectory(path.c_str()))
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
// void	_delete_(request& requestObj){
// 	requestObj.setStatusCode("204 No Content");
// 	string path = requestObj.loc.getRoot() + requestObj.getrequestURI();
// 	if (checkExistance(requestObj))
// 			return ;
// 	if (isDirectory(path.c_str())){//directory
// 		if (!endsWithSlash(path)){//no slash
// 			cout << "slash makaynash\n";
// 			codeNpath(requestObj,"409 Conflict",
// 				errorPageTamplate("409, Conflict.").c_str());
// 			return ;
// 		}
// 		else {//with slash
// 			DIR* dir = opendir(path.c_str());
// 			struct dirent* entry;
// 			vector<const char*> delFiles;
// 			bool success = true;
// 			while ((entry = readdir(dir)) != NULL)
// 			{
// 				if (strcmp(entry->d_name, ".") != 0
// 					&& strcmp(entry->d_name, "..") != 0)
// 				{
// 					if (entry->d_type == DT_REG){
// 						if (unlink((path + entry->d_name).c_str()) != 0)
// 						{
// 							success = false;
// 							break;
// 						}
// 					}
// 					else if (entry->d_type == DT_DIR){
// 						if (rmdir((path + entry->d_name).c_str()) != 0)
// 						{
// 							success = false;
// 							break;
// 						}
// 					}
// 				}
// 			}
// 			if (!success)
// 			{
// 				if (access(path.c_str(), W_OK) == 0)//has write access
// 				{
// 					codeNpath(requestObj,"403 Forbidden",
// 						errorPageTamplate("403, Forbidden.").c_str());
// 						return ;
// 				}
// 				else
// 				{//doesnt have write access
// 					codeNpath(requestObj,"503 Internal Server Error",
// 						errorPageTamplate("503 , Internal Server Error.").c_str());
// 						return ;
// 				}
// 			}
// 		}
// 	}
// 	else {//is file
// 		if (unlink(requestObj.getFilePath().c_str()) != 0) {
// 			codeNpath(requestObj,"403 Forbidden",
// 			errorPageTamplate("403, Forbidden.").c_str());
// 			return ;
// 		}
// 	}
// }
