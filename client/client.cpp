#include "../server.hpp"
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
//client_class

void	client::setclient(char* r, int connection_socket, server& _server){
	// string buff(r);
	requestObj.parseRequest(r, _server);
    cout << RED << "this range of bytes: " << requestObj.getBytesRange() << RESET_TEXT << endl;
    // responseObj.setcontentType(requestObj.getContentType());
    // responseObj.setURI(requestObj.getrequestURI());
    responseObj.set_res(connection_socket, requestObj);
    // responsestring = responseObj.get_res();
}

string client::getresponse(){
    return responsestring;
}
