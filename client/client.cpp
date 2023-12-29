#include "../server.hpp"
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
//client_class
void	client::setclient(char* r){
	string buff(r);
	requestObj.parseRequest(buff);
    responseObj.setcontentType(requestObj.getContentType());
    responseObj.setURI(requestObj.getrequestURI());
    responseObj.set_res();
    responsestring = responseObj.get_res();
}

string client::getresponse(){
    return responsestring;
}
