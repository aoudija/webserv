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
	StaunchOne.portSetter("60");
	StaunchOne.set_isdefault(1);
	vector<server> servers;
	servers.push_back(StaunchOne);
	return servers;
}

// Function to create an HTTP response with HTML content and an image
string createHtmlImageResponse() {

    static int flag;

    std::ifstream htmlFile("index.html");
    if (!htmlFile.is_open()) {
        std::cerr << "Error opening HTML file\n";
        // Log the error, return an error response, or handle it appropriately
    }

    string buffer,c;
    while (std::getline(htmlFile, c)){
        buffer += c + "\n";
    }
    // Load HTML content
    std::string htmlContent(buffer);

    std::ifstream imageFile("img.png");
    if (!imageFile.is_open()) {
        std::cerr << "Error opening image file\n";
        // Log the error, return an error response, or handle it appropriately
    }
    c = "";
    std::string imageContent;
    while (std::getline(imageFile, c))
        imageContent += c;
    // Load image content


    std::string msg;
    if (flag == 0)
    {
        std::cout << "FIRST " << std::endl;
        msg = "HTTP/1.1 200 OK\r\n"
            "Content-Length: " + std::to_string(htmlContent.size()) + "\r\n"
            "Content-Type: text/html\r\n"
            "\r\n" + htmlContent;
        flag = 1;
    }
    else if (flag == 1)
    {
        std::cout << "SECONDE " << std::endl;
        msg = "HTTP/1.1 200 OK\r\n"
        "Content-Length: " + std::to_string(imageContent.size()) + "\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n";
        // + imageContent
        flag = 0;
    }
    // Create the HTTP response
    // cout << "ktaba : " << msg << endl;
    return msg;
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

	//multiplexing vz.0
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

				servers[i].set_response(createHtmlImageResponse());
                // cout << YELLOW << createHtmlImageResponse().c_str() << RESET_TEXT << endl;
				int bytes_sent = send(servers[i].get_sconncetion(),
				createHtmlImageResponse().c_str(), strlen(createHtmlImageResponse().c_str()), 0);//response
				
				close(servers[i].get_sconncetion());
			}
		}
	}
}

