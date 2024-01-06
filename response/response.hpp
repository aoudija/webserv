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
	int		offset;
	string	res;
	public:
	response(){
		offset = 0;
	}
		void	set_res(int connection_socket, request&);
		string	get_res();
		void	setcontentType(string contenttype);
};

#endif
