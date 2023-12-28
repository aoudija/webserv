#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>
#include "Location.hpp"
#include <sstream>


class Location;

class Server
{
private:
	std::string					server_name;
	std::string					port;
	std::string					hostname;
	std::string					root;
	std::string					index;
	int							client_body_limit;
	bool						autoindex;
	std::vector< std::string>	allow_methods;
	std::map<int, std::string>	error_page;
	std::vector<Location>		locations;

	void	checkfirstline(std::string str, int line);
	void	checklastline(std::string str, int line, int firstline);
	void	seter(std::string str, int line);
	void	set_value(std::vector<std::string> list, int token, int line);
	void	Myhostport(std::vector<std::string> list, int line);
	std::vector<std::string> splitString(const std::string& input, const std::string& delm) ;
	std::string throwmessage(int number, const std::string& str);
	std::string withoutsemicolon(std::string str);
	bool isInMyList(const std::string& target, int &token);
	bool isWhitespace(const std::string& str);

	
	void		Myserver_name(std::vector<std::string> list, int line);
	void		Myroot(std::vector<std::string> list, int line);
	void		Myindex(std::vector<std::string> list, int line);
	void		Myclient_body_limit(std::vector<std::string> list, int line);
	void		Myautoindex(std::vector<std::string> list, int line);
	void		Myallow_methods(std::vector<std::string> list, int line);
	void		Myerror_page(std::vector<std::string> list, int line);
	void		Mylocations(std::vector<Location>&);
	void		setmylocation(std::map<int, std::string>::const_iterator &it, std::map<int, std::string> &server);
public:
	Server();
	~Server();
	void		setPort(std::string);
	void		setHostname(std::string);
	void		setServerName(std::string);
	void		setRoot(std::string);
	void		setIndex(std::string);
	void		setClientBodyLimit(int);
	void		setAutoindex(bool);
	void		setAllowMethods(std::string);
	void		setErrorPage(int, std::string);
	void		setLocations(Location&);

	void	parse(std::map<int, std::string>&);
	std::string	getServerName(void) const;
	std::string	getPort(void) const;
	std::string	getHostname(void) const;
	std::string	getRoot(void) const;
	std::string	getIndex(void) const;
	int			getClientBodyLimit(void) const;
	bool		getAutoindex(void) const;
	std::vector< std::string>		getAllowMethods(void) const;
	std::map<int, std::string>		getErrorPage(void) const;
	std::vector<Location>			getLocations(void) const;
};

#endif