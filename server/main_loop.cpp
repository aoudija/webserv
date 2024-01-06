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
		for (size_t i = 0; i < _si.allSockets.size();i++)
			FD_SET(_si.allSockets[i], &sockets);
		copy = sockets;
		int maxfd = *(std::max_element(_si.allSockets.begin(), _si.allSockets.end()));
		if (select(maxfd + 1, &copy, 0, 0, 0) < 0){
			perror("select failed");
			exit(EXIT_FAILURE);
		}
		for (size_t i = 0; i < servers.size();i++){
			cout << "hello\n";
			for (size_t j = 0; j < _si.allSockets.size();j++){
			if (FD_ISSET(_si.allSockets[j], &copy)){
				if (servers[i].serverallsockets[j] == servers[i].get_slistener()){
					cout << RED << "this is fd from listners: "
						<< servers[i].serverallsockets[j] << RESET_TEXT << endl;
					servers[i].set_sconnection(accept(servers[i].get_slistener(),
							(sockaddr*)&client_addr, &clientaddr_len));

					servers[i].serverallsockets.push_back(servers[i].get_sconncetion());
					_si.allSockets.push_back(servers[i].get_sconncetion());
				}
				else{
				int bytesrecv = read(_si.allSockets[j], request_string, 1024);
				if (!bytesrecv || bytesrecv < 0){
					cout << "read error " << endl;
					close(_si.allSockets[j]);
					_si.allSockets.erase(std::find(_si.allSockets.begin(),
						_si.allSockets.end(), _si.allSockets[j]));
					break ;
				}
				cout << WHITE << "received: " << bytesrecv << RESET_TEXT << endl;
				printf("\033[1;37m'%.*s'\033[0m", bytesrecv, request_string);
				int flag = 0;//to handle chucked request
				for(it = clients.begin();it != clients.end();it++){
					if (memcmp(&client_addr, &it->first, sizeof(client_addr)) == 0){
						cout << "already in" << endl;
						it->second.setclient(request_string, _si.allSockets[j],
							servers[i]);
						flag = 1;
						break ;
					}
				}
				if (!flag){
					client temp;
					temp.setclient(request_string, _si.allSockets[j], servers[i]);
					clients.push_back(std::make_pair(client_addr, temp));
				}
				/*--------------*/
				cout << GREEN << "RESPONSE SENT " << RESET_TEXT << endl;
				}
			}
			}
				// close(servers[i].get_sconncetion());
		}
	}
}