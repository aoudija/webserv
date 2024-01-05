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
	// string http_version, status_code;
	// map<string, string> header_fields;
	// string	body;
	string uri, contentType;
	string res;
	public:
		void	set_res(int connection_socket, request&);
		string	get_res();
		void	setURI(string uri);
		void	setcontentType(string contenttype);
};

#endif
