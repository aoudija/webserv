#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include <sstream>
#include <fstream>
#include "../request/request.hpp"
class request;

class response{
	string	res;
	bool	allFileSent;
	public:
		response(){
			allFileSent = 0;
		}
		bool getallfs(){
			return allFileSent;
		}
		int	set_res(int connection_socket, size_t offset, request&);
		string	get_res();
};

#endif
