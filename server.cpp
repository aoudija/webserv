#include "server.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;

/*server_class*/
void	server::portSetter(string prt){
	port = prt;
}

void	server::set_slistener(int s){
	_s_listener = s;
}

void	server::set_sconnection(int s){
	_s_connection = s;
}

string	server::portGetter(){
	return port;
}

int	server::get_slistener(){
	return _s_listener;
}

int	server::get_sconncetion(){
	return _s_connection;
}

void	server::set_ip(string ip){
	ip  = ip;
}

string	server::get_ip(){
	return ip;
}


/*serersInfos_class*/
serversInfos::serversInfos(const vector<server>& servers){
	this->servers = servers;
}

void	serversInfos::closeListeners(){
	vector<server>::iterator it;
	for (it = servers.begin(); it < servers.end();it++)
		close(it->get_slistener());
}

vector<server> serversInfos::get_servers(){
	return servers;
}

void	server::set_request(string rq){
	request = rq;
}

void	server::set_response(string rs){
	response = rs;
}

string	server::get_request(){
	return request;
}

string	server::get_response(){
	return response;
}


void serversInfos::SetListener(){
	vector<server>::iterator it;
	for(it = servers.begin();it < servers.end();it++){
		struct addrinfo server_addr, *cn;
		bzero(&server_addr, sizeof(server_addr));
		server_addr.ai_family = AF_UNSPEC;
		server_addr.ai_socktype = SOCK_STREAM;
		server_addr.ai_flags = AI_PASSIVE;
		if (getaddrinfo(it->get_ip().c_str(), (it->portGetter()).c_str(),
			&server_addr, &cn) != 0)
		{	cout << RED << "getaddrinfo() failed" << RESET_TEXT << endl;
			exit(EXIT_FAILURE);
		}
		//mainpart
		it->set_slistener(socket(cn->ai_family,
			cn->ai_socktype, cn->ai_protocol));
		
		if (bind(it->get_slistener(), cn->ai_addr,cn->ai_addrlen) < 0)
		{
			cout << RED << "bind() failed" << RESET_TEXT << endl;
			freeaddrinfo(cn);
			closeListeners();
			exit(EXIT_FAILURE);
		}
		freeaddrinfo(cn);
		if (listen(it->get_slistener(), 20) < 0)
		{
			cout << RED << "listen() failed" << RESET_TEXT << endl;
			closeListeners();
			exit(EXIT_FAILURE);
		}
		cout << "port: " << it->portGetter();
		cout << " listening on socket "
			<< it->get_slistener() << endl;
	}
}


