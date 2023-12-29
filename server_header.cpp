#include "server_header.hpp"

using std::string;
using std::cout;
using std::endl;

void	serverInfo::closeListeners(){
	for (int i = 0; i < listeners.size();i++)
		close(listeners[i]);
}

void	serverInfo::SetPorts(int n, const char* port){
    mapPort[n] = port;
}

const vector<int>&	serverInfo::getListenres(){
	return listeners;
}

const map<int, const char*>& serverInfo::getMapPort(){
	return mapPort;
}

void serverInfo::SetListener(int index){
	
	struct addrinfo server_addr, *cn;

	server_addr.ai_family = AF_UNSPEC;
	server_addr.ai_socktype = SOCK_STREAM;
	server_addr.ai_flags = AI_PASSIVE;

	getaddrinfo(0, mapPort[index], &server_addr, &cn);
	listeners[index] = socket(cn->ai_family,
		cn->ai_socktype, cn->ai_protocol);
	if (bind(listeners[index], cn->ai_addr,cn->ai_addrlen) < 0)
	{
	    cout << RED << "bind() failed" << RESET_TEXT << endl;
	    freeaddrinfo(cn);
	    closeListeners();
	    exit(EXIT_FAILURE);
	}
	freeaddrinfo(cn);
	if (listen(listeners[index], 20) < 0)
	{
	    cout << RED << "listen() failed" << RESET_TEXT << endl;
	    closeListeners();
	    exit(EXIT_FAILURE);
	}
}


