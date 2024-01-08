#include "../server.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;

void	main_loop(vector<server> Confservers){
	serversInfos	_si(Confservers);
	_si.SetListener();
	vector<server> servers = _si.get_servers();
	vector<int> toRemove;
	map<int, client> clients;
	struct sockaddr_storage client_addr;
	socklen_t clientaddr_len = sizeof(client_addr);
	char request[1024];

	//multiplexing v3.2
	while (true){
		struct pollfd pfds[_si.allSockets.size()];
		for (size_t i = 0; i < _si.allSockets.size();i++){
			pfds[i].fd = _si.allSockets[i];
			pfds[i].events = POLLIN | POLLHUP | POLLOUT;
		}
		poll(pfds, _si.allSockets.size(), -1);
		size_t i = 0;
		for (; i < servers.size();i++){
			size_t x = 0;
			for (; x < _si.allSockets.size();x++){
				int fd = pfds[x].fd;
				if (pfds[x].revents & POLLIN){
					if (fd == servers[i].get_slistener()){//listener
						cout << "listener: " << fd << endl;
						servers[i].set_sconnection(accept(servers[i].get_slistener(),
								(sockaddr*)&client_addr, &clientaddr_len));
						fcntl(servers[i].get_sconncetion(),F_SETFL,O_NONBLOCK,FD_CLOEXEC);
						cout << "connection socket: " << servers[i].get_sconncetion()<<endl;;
						_si.allSockets.push_back(servers[i].get_sconncetion());
						servers[i].connectionsockets.push_back(servers[i].get_sconncetion());
						servers[i].serversockets.push_back(servers[i].get_sconncetion());
						client temp;
						clients[servers[i].get_sconncetion()] = temp;
					}
					else if (std::find(servers[i].connectionsockets.begin(),
						servers[i].connectionsockets.end(), fd)
							!= servers[i].connectionsockets.end() && !clients[fd].getTookreques()){//read request 	&& clients[fd].getfilesent()
						int r = read(fd, request, 1024);
						cout << "Received " << r << " bytes." << endl;
						printf("\033[1;37m%.*s\033[0m", r, request);
						clients[fd].set_request(request, servers[i]);
					}
				}
				else if (!(pfds[x].revents & POLLHUP) && fd != servers[i].get_slistener()
					&& clients[fd].getTookreques()){
					if (std::find(servers[i].connectionsockets.begin(),
					servers[i].connectionsockets.end(), fd) != servers[i].connectionsockets.end()){//respond to request
						clients[fd].set_response(fd);
						cout<<GREEN<<"response_sent"<<RESET_TEXT<<endl;
						if (clients[fd].getfilesent()){
							close(fd);
							_si.allSockets.erase(std::find(_si.allSockets.begin(),_si.allSockets.end(), fd));
						}
					}
				}
			}
		}
	}
}