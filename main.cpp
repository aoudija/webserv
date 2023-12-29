#include "server.hpp"
#include "aubb/Config.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

vector<server>	mini_parsing(){
	server	StaunchOne;
	server	StaunchTwo;
	server	StaunchThree;

	StaunchOne.set_ip("localhost");
	StaunchOne.portSetter("8080");
	StaunchOne.set_isdefault(1);
	// StaunchTwo.set_ip("127.0.0.1");
	// StaunchTwo.portSetter("8081");
	// StaunchTwo.set_isdefault(1);
	// // StaunchTwo.set_my_default(0);
	// StaunchThree.set_ip("0.0.0.0");
	// StaunchThree.portSetter("8083");
	// StaunchThree.set_isdefault(1);
	// // StaunchThree.set_my_default(0);
	vector<server> servers;
	servers.push_back(StaunchOne);
	// servers.push_back(StaunchTwo);
	// servers.push_back(StaunchThree);
	return servers;
}

int main(int ac, char **av){
	(void)ac;
	vector<pair<struct sockaddr_storage, client> > clients;
	vector<pair<struct sockaddr_storage, client> >::iterator it;

	Config	conf(av[1]);
	vector<server> servers;
	int bytes_sent;
	char request_string[1024];
	int bytes_received;
	// _init_servers(servers);
    // serversInfos	_si( mini_parsing());
    serversInfos	_si( conf.Servers);

	_si.SetListener();
	servers = _si.get_servers();
	// servers = conf.Servers;
	struct sockaddr_storage client_addr;
	socklen_t clientaddr_len = sizeof(client_addr);
	string RESPONSE;


	//multiplexing v3.0
	while (true){
		fd_set sockets, copy;
		FD_ZERO(&sockets);

		for (size_t i = 0; i < servers.size();i++)
			FD_SET(servers[i].get_slistener(), &sockets);
		copy = sockets;
		if (select(servers[servers.size() - 1].get_slistener() + 1
			, &copy, 0, 0, 0) < 0){
			cout << RED << "select() failed" << RESET_TEXT << endl;
	    	exit(EXIT_FAILURE);
		}
		for (size_t i = 0; i < servers.size();i++){
			if (FD_ISSET(servers[i].get_slistener(), &copy)){
				cout << RED << "socket listenner: " << servers[i].get_slistener() << RESET_TEXT << endl;
				servers[i].set_sconnection(accept(servers[i].get_slistener(),
						(sockaddr*)&client_addr,
						&clientaddr_len));

				cout << CYAN << "CONNECTION SOCKET : " << servers[i].get_sconncetion()
					<< RESET_TEXT << endl;
				cout << GREEN << "CONNECTION ACCEPTED .." << RESET_TEXT << endl;
				/*--------------*/

				bytes_received = recv(servers[i].get_sconncetion(),
					request_string, 1024, 0);//request
				int flag = 0;
				for(it = clients.begin();it != clients.end();it++){
					if (memcmp(&client_addr, &it->first, sizeof(client_addr)) == 0){
						it->second.setclient(request_string);
						RESPONSE = it->second.getresponse();
						flag = 1;
						break ;
					}
				}
				if (!flag){
					client temp;
					temp.setclient(request_string);
					clients.push_back(std::make_pair(client_addr, temp));
					RESPONSE = temp.getresponse();
				}
				cout << "Received " << bytes_received << " bytes." << endl;
				printf("\033[1;37m%.*s\033[0m", bytes_received, request_string);

				cout << YELLOW << "SENDING RESPONSE ..." << RESET_TEXT << endl;

				bytes_sent = send(servers[i].get_sconncetion(),
					RESPONSE.c_str(), strlen(RESPONSE.c_str()), 0);//response1
				cout << bytes_sent << '/' << strlen(RESPONSE.c_str()) << " sent" << endl;
				close(servers[i].get_sconncetion());
			}
		}
	}
}
