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
		for (size_t i = 0; i < servers.size();i++){
			FD_SET(servers[i].get_slistener(), &sockets);
		}
		// memcpy(&copy, &sockets, sizeof(sockets));
		copy = sockets;
		if (select(servers[servers.size() - 1].get_slistener() + 1
			, &copy, 0, 0, 0) < 0){
			perror("select failed");
			exit(EXIT_FAILURE);
		}
		for (size_t i = 0; i < servers.size();i++){
			// cout << "hello\n";
			if (FD_ISSET(servers[i].get_slistener(), &copy)){
				cout<<RED<<"socket listenner: "<< servers[i].get_slistener()
					<< RESET_TEXT << endl;
				servers[i].set_sconnection(accept(servers[i].get_slistener(),
						(sockaddr*)&client_addr,
						&clientaddr_len));
				cout << CYAN << "CONNECTION SOCKET : " << servers[i].get_sconncetion()
					<< RESET_TEXT << endl;
				cout << GREEN << "CONNECTION ACCEPTED .." << RESET_TEXT << endl;
				/*--------------*/
				int bytesrecv = read(servers[i].get_sconncetion(), request_string, 1024);
				if (!bytesrecv || bytesrecv < 0){
					close(servers[i].get_sconncetion());
					break ;
				}
				cout << WHITE << "received: " << bytesrecv << RESET_TEXT << endl; 
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
					clients.push_back(std::make_pair(client_addr, temp));
				}
				/*--------------*/
				cout << GREEN << "RESPONSE SENT " << RESET_TEXT << endl;
			}
			close(servers[i].get_sconncetion());
		}
	}
}