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
	request		&MyRequest;
	char		**arg;
	std::string filename;
	std::string filebody;
	std::string fileerr;
	void	parseHeader(std::vector<std::string> header, size_t len);
public:
	// Cgi(server &serv, Location &location, client &cln);
	Cgi(server &serv, request &req);
	~Cgi();
	std::string	header;
	std::string	body;
	void	init();
	int		exe();
	void	set_arg();
	int ParseAll();
};

#endif