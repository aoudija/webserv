#include "../server.hpp"
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
//client_class

void	client::setclient(char* r, int connection_socket, server& _server){
	requestObj.parseRequest(r, _server);
    cout << "hello maffuckers\n";
    responseObj.set_res(connection_socket, requestObj);
}

string client::getresponse(){
    return responsestring;
}
