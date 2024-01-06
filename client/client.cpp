#include "../server.hpp"
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
//client_class

void	client::setclient(char* r, int connection_socket, server& _server){
	requestObj.parseRequest(r, _server);
    offset = responseObj.set_res(connection_socket, offset, requestObj);
    cout << RED << offset <<RESET_TEXT<< endl;
    filesent = responseObj.getallfs();
}

string client::getresponse(){
    return responsestring;
}

bool	client::getfilesent(){
    return filesent;
}