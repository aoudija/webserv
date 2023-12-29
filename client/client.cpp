#include "../server.hpp"

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
