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
#include <sstream>
#include <fstream>

#define SOCKET int

using std::map;
using std::vector;
using std::string;

class server{
	string	port, serverName, request, response, ip;
	bool is_default;
	int	my_default;
	public:
		void	portSetter(string prt);
		void	set_slistener(int s);
		void	set_sconnection(int s);
		void	set_request(string rq);
		void	set_response(string rs);
		string	get_request();
		string	get_response();
		string	portGetter();
		int		get_slistener();
		int		get_sconncetion();
		void	set_ip(string ip);
		string	get_ip();
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

/*server {
    # listen on port 80
    listen 80;

    # server name
    server_name example.com;

    # default location
    location / {
        # root directory
        root /var/www/html;

        # index file
        index index.html;
    }
}*/


#endif