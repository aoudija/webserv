#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include <sstream>
#include <fstream>

class response{
	// string http_version, status_code;
	// map<string, string> header_fields;
	// string	body;
	string uri, contentType;
	string res;
	public:
		void	set_res();
		string	get_res();
		void	setURI(string uri);
		void	setcontentType(string contenttype);
};

#endif
