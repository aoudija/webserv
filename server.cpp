#include "server_header.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;

int main(){
    struct addrinfo server_addr, *cn;

    server_addr.ai_family = AF_UNSPEC;
    server_addr.ai_socktype = SOCK_STREAM;
    server_addr.ai_flags = AI_PASSIVE;

	getaddrinfo(0, "80", &server_addr, &cn);
	int listen_socket = socket(cn->ai_family, cn->ai_socktype, cn->ai_protocol);
	cout << "listening socket : " << listen_socket << endl;
	if (bind(listen_socket, cn->ai_addr,cn->ai_addrlen) < 0)
	{
	    cout << RED << "bind() failed" << RESET_TEXT << endl;
	    freeaddrinfo(cn);
	    close(listen_socket);
	    exit(EXIT_FAILURE);
	}
	freeaddrinfo(cn);
	if (listen(listen_socket, 20) < 0)
	{
	    cout << RED << "listen() failed" << RESET_TEXT << endl;
	    close(listen_socket);
	    exit(EXIT_FAILURE);
	}

	struct sockaddr_storage client_addr;
	socklen_t clientaddr_len = sizeof(client_addr);

	char request[1024];
	int connection_socket, bytes_received;

	//multiplexing v0.1
	while (true){
		fd_set sockets, copy;
		FD_ZERO(&sockets);
		FD_SET(listen_socket, &sockets);
		copy = sockets;
		
		select(listen_socket + 1, &copy, 0, 0, 0);
		if (FD_ISSET(listen_socket, &copy)){
			connection_socket = accept(listen_socket,
					(sockaddr*)&client_addr, 
					&clientaddr_len);

			cout << CYAN << "CONNECTION SOCKET : " << connection_socket << RESET_TEXT << endl;
			cout << GREEN << "CONNECTION ACCEPTED .." << RESET_TEXT << endl;
			bytes_received = recv(connection_socket, request, 1024, 0);
			cout << "Received " << bytes_received << " bytes." << endl;
			printf("\033[1;37m%.*s\033[0m", bytes_received, request);

			cout << YELLOW << "SENDING RESPONSE ..." << RESET_TEXT << endl;
			const char *response =
				"HTTP/1.1 200 OK\r\n"
				"Connection: close\r\n"
				"Content-Type: text/html\r\n\r\n"
				"<h1> HELLO WORLD </h1>";
			int bytes_sent = send(connection_socket, response, strlen(response), 0);
			cout << bytes_sent << '/' << strlen(response) << " sent" << endl;
			
			close(connection_socket);
		}
	}

    close(listen_socket);
}
