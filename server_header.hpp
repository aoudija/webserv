#ifndef SEVER_HEADER_HPP
#define SEVER_HEADER_HPP

#define BLACK "\033[1;30m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define WHITE "\033[1;37m"
#define RESET_TEXT "\033[0m"

#include <netdb.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

using std::map;
using std::vector;
using std::string;

class serverInfo
{
    private:
		map<int, const char*>	mapPort;
		vector<int>				listeners;
    public:
		void		SetPorts(int n, const char* port);
		void		SetListener(int index);
		void		closeListeners();
		const vector<int>&	getListenres();
		const map<int, const char*>& getMapPort();
};



#endif