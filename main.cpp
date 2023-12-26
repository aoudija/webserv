#include "server.hpp"

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
	StaunchOne.portSetter("80");
	StaunchOne.set_isdefault(1);
	StaunchTwo.set_ip("127.0.0.1");
	StaunchTwo.portSetter("80");
	StaunchTwo.set_isdefault(0);
	StaunchTwo.set_my_default(0);
	StaunchThree.set_ip("0.0.0.0");
	StaunchThree.portSetter("80");
	StaunchThree.set_isdefault(0);
	StaunchThree.set_my_default(0);
	vector<server> servers;
	servers.push_back(StaunchOne);
	servers.push_back(StaunchTwo);
	servers.push_back(StaunchThree);
	return servers;
}

int main(){
	vector<server> servers;
	static int image;
	servers = mini_parsing();
	// _init_servers(servers);
    serversInfos	_si(servers);

	_si.SetListener();
	servers = _si.get_servers();
	struct sockaddr_storage client_addr;
	socklen_t clientaddr_len = sizeof(client_addr);

	char request[1024];
	int connection_socket, bytes_received;

	//multiplexing v3.0
	while (true){
		fd_set sockets, copy;
		FD_ZERO(&sockets);
		
		for (int i = 0; i < servers.size();i++){
			cout << "setting socket_listener: " << servers[i].get_slistener()
				<< endl;
			FD_SET(servers[i].get_slistener(), &sockets);
			cout << servers.size() << endl;
		}
		copy = sockets;
		if (select(servers[servers.size() - 1].get_slistener() + 1
			, &copy, 0, 0, 0) < 0){
			cout << RED << "select() failed" << RESET_TEXT << endl;
	    	exit(EXIT_FAILURE);
		}
		for (int i = 0; i < servers.size();i++){
			if (FD_ISSET(servers[i].get_slistener(), &copy)){

				servers[i].set_sconnection(accept(servers[i].get_slistener(),
						(sockaddr*)&client_addr, 
						&clientaddr_len));

				cout << CYAN << "CONNECTION SOCKET : " << servers[i].get_sconncetion()
					<< RESET_TEXT << endl;
				cout << GREEN << "CONNECTION ACCEPTED .." << RESET_TEXT << endl;
				/*--------------*/
				
				bytes_received = recv(servers[i].get_sconncetion(), request, 1024, 0);//request

				servers[i].set_request(request);
				cout << "Received " << bytes_received << " bytes." << endl;
				printf("\033[1;37m%.*s\033[0m", bytes_received, request);

				cout << YELLOW << "SENDING RESPONSE ..." << RESET_TEXT << endl;
				
				std::ifstream imgfile("get_ready_for_work.png");
				std::string buffer, c;
				while (std::getline(imgfile, c))
					buffer += c;
				const char *response =
				"HTTP/1.1 200 OK\r\n"\
				"Connection: close\r\n"\
				"Content-Type: text/html\r\n\r\n"\
				"<h1> HELLO WORLD </h1><img src=\"get_ready_for_work.png\">";
				servers[i].set_response(response);

				int bytes_sent = send(servers[i].get_sconncetion(),
					response, strlen(response), 0);//response

				cout << bytes_sent << '/' << strlen(response) << " sent" << endl;
				if (image){
					bytes_sent = send(connection_socket, buffer.c_str(),
						strlen(buffer.c_str()), 0);
					cout << "image---> " << bytes_sent << '/' << strlen(buffer.c_str()) << " sent**" << endl;
				}
				
				close(connection_socket);
				image++;
			}
		}
	}
}
