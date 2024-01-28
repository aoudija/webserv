#ifndef CGI_HPP
# define CGI_HPP

#include <iostream>
#include <vector>
#include <map>
#include "../server.hpp"
#include "../request/request.hpp"

class	client;
class Cgi
{
private:
	char		**env;
	server		&MyServer;
	Location	&MyLocation;
	request		&MyRequest;
	char		**arg;
	void	parseHeader(std::vector<std::string> header, size_t len);
public:
	// Cgi(server &serv, Location &location, client &cln);
	Cgi(server &serv, Location &location, request &req);
	~Cgi();
	std::string	header;
	std::string	body;
	void	init();
	int		exe();
	void	set_arg();
};

#endif