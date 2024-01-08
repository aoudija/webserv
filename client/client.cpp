#include "../server.hpp"
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
//client_class

void	client::set_request(char* r, server& _server){
    requestObj.parseRequest(r, _server);
    tookrequest = 1;
}

void	client::set_response(int connection_socket){
    offset = responseObj.set_res(connection_socket, offset, requestObj);
    cout << RED << offset <<RESET_TEXT<< endl;
    filesent = responseObj.getallfs();
    if (filesent == 1)
        tookrequest = 0;
}



string client::getresponse(){
    return responsestring;
}

bool	client::getfilesent(){
    return filesent;
}

bool	client::getTookreques(){
    return tookrequest;
}