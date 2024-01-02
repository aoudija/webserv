#include "../server.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;

void	main_loop(vector<server> Confservers){
	char request_string[1024];
	serversInfos	_si(Confservers);
	_si.SetListener();
	vector<server> servers = _si.get_servers();
	
	vector<pair<struct sockaddr_storage, client> > clients;
	vector<pair<struct sockaddr_storage, client> >::iterator it;
	struct sockaddr_storage client_addr;
	socklen_t clientaddr_len = sizeof(client_addr);
	string RESPONSE;

	//multiplexing v5.0
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

				int bytesrecv = recv(servers[i].get_sconncetion(),
					request_string, 1024, 0);//request
				printf("\033[1;37m%.*s\033[0m", bytesrecv, request_string);
				/*--------------*/
				int flag = 0;//to handle chucked request
				for(it = clients.begin();it != clients.end();it++){
					if (memcmp(&client_addr, &it->first, sizeof(client_addr)) == 0){
						it->second.setclient(request_string, servers[i].get_sconncetion(),
							servers[i]);
						flag = 1;
						break ;
					}
				}
				if (!flag){
					client temp;
					temp.setclient(request_string, servers[i].get_sconncetion(), servers[i]);
					cout << RED << "WHERE ARE WE" << RESET_TEXT << endl;
					clients.push_back(std::make_pair(client_addr, temp));
				}
				/*--------------*/

				cout << YELLOW << "SENDING RESPONSE ..." << RESET_TEXT << endl;
				close(servers[i].get_sconncetion());
			}
		}
	}
}