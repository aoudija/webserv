#ifndef SERVER_HPP
#define SERVER_HPP

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
#include <utility>
#include <sstream>
#include <fstream>
#include "request/request.hpp"
#include "response/response.hpp"
#include "aubb/Location.hpp"
#include "aubb/Config.hpp"


using std::map;
using std::vector;
using std::string;
using std::pair;

class server{
	string	port, serverName, request, response, ip;
	bool is_default;
	int my_default, _s_listener, _s_connection;
	public:
		void	portSetter(string prt);
		void	set_my_default(int index);
		void	set_isdefault(bool b);
		void	set_slistener(int s);
		void	set_sconnection(int s);
		void	set_request(string rq);
		void	set_response(string rs);
		void	set_ip(string ip);
		string	get_request();
		string	get_response();
		string	portGetter();
		string	get_ip();
		bool	get_isdefault();
		int		get_my_default();
		int		get_slistener();
		int		get_sconncetion();
};

class serversInfos
{
	private:
		vector<server>			servers;
	public:
		serversInfos(const vector<server>& servers);
		void		SetListener();
		void		closeListeners();
		vector<server> get_servers();
};

//REQUEST_CLASS
// class request
// {
// 	string method;
// 	string requestURI;
// 	string httpVersion;
// 	map<std::string, std::string> headerFields;
// 	string body;

// public:
// 	request();
// 	request(std::string req);
// 	request(const request &other);
// 	request& operator=(const request& other);
// 	std::string getMethod();
// 	void checkRequestLine(std::string request);
// 	void checkHeaderFields(std::string headerFiles);
// 	void parseRequest(std::string request);
// 	void checkBody(std::string body);
// 	string getrequestURI();
// 	string getContentType();
// };

//RESPONSE_CLASS
// class response{
// 	// string http_version, status_code;
// 	// map<string, string> header_fields;
// 	// string	body;
// 	string uri, contentType;
// 	string res;
// 	public:
// 		void	set_res();
// 		string	get_res();
// 		void	setURI(string uri);
// 		void	setcontentType(string contenttype);
// };

//Client class
class client
{
	request requestObj;
	response responseObj;
	string responsestring;
	public:
		void	setclient(char* r);
		string getresponse();
};


#endif