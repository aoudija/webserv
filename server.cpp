
#include "Server.hpp"


std::vector<std::string> Server::splitString(const std::string& input, const std::string& delm) {
    std::vector<std::string> tokens;
    std::string::size_type start = 0;
    std::string     token;

    while (start < input.length()) {
        // Find the first non-whitespace character
        std::string::size_type nonWhitespace = input.find_first_not_of(delm.c_str(), start);

        // Find the next whitespace character
        std::string::size_type end = input.find_first_of(delm.c_str(), nonWhitespace);

        // Extract the substring between nonWhitespace and end
        if (nonWhitespace != std::string::npos) {
            if (end == std::string::npos)
                token = input.substr(nonWhitespace, end);
            else
                token = input.substr(nonWhitespace, end - nonWhitespace);
            tokens.push_back(token);
        }

        // Move to the next character after the found whitespace
        if (end == std::string::npos)
            start = end;
        else
            start = end + 1;
    }

    return tokens;
}

Server::Server(/* args */)
{
}

Server::~Server()
{
}

std::string Server::throwmessage(int number, const std::string& str) {
    std::stringstream ss;
    ss << number;
    return  "config:line:" + ss.str() + ": " + str;
}

std::string Server::withoutsemicolon(std::string str){
	if (!str.empty()) {
        size_t lastNonSemicolon = str.find_last_not_of(';');
        if (lastNonSemicolon != std::string::npos && lastNonSemicolon < str.length() - 1) {
            return str.substr(0, lastNonSemicolon + 1);
        }
    }

    return str;
}



bool Server::isInMyList(const std::string& target, int &token) {
	std::vector<std::string> lst;
    lst.push_back("server_name");
    lst.push_back("listen");
    lst.push_back("root");
    lst.push_back("allow_methods");
    lst.push_back("autoindex");
    lst.push_back("index");
    lst.push_back("client_body_limit");
    lst.push_back("error_page");
	for (size_t i = 0; i < lst.size(); i++){
		if (lst[i].compare(target) == 0){
			token = i + 1;
			break;
		}
	}
	return std::find(lst.begin(), lst.end(), target) != lst.end();
}

bool Server::isWhitespace(const std::string& str) {
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        if (!std::isspace(static_cast<unsigned char>(*it))) {
            return false; // Found a non-whitespace character
        }
    }
    return true; // Only whitespace characters found
}


void	Server::Myhostport(std::vector<std::string> list, int line){
	if (!list[list.size() - 1].compare(";"))
		list.pop_back();
	if (list.size() != 2 || list.empty())
			throw std::invalid_argument(throwmessage(line, "invalid input hostname:port."));

	// Find the position of ':'
    size_t colonPos = list[1].find(':');

    // Check if ':' exists in the string
    if (colonPos == std::string::npos)
        throw std::invalid_argument(throwmessage(line, "Error: Missing colon in input string."));

	// Extract hostname and port substrings
    setHostname(list[1].substr(0, colonPos));
    std::string portStr = withoutsemicolon(list[1].substr(colonPos + 1));

    // Convert port string to integer
    char* endPtr;
    long parsedPort = std::strtol(portStr.c_str(), &endPtr, 10);

    // Check for conversion errors
    if (*endPtr != '\0' || parsedPort < 0 || parsedPort > 65535)
        throw std::invalid_argument(throwmessage(line, "Error: Invalid port number."));

	setPort(portStr);
}


void	Server::set_value(std::vector<std::string> list, int token, int line){
	
	switch (token)
	{
	case 1:
		Myserver_name(list, line);
		break;
	case 2:
		Myhostport(list, line);
		break;
	case 3:
		Myroot(list, line);
		break;
	case 4:
		Myallow_methods(list, line);
		break;
	case 5:
		Myautoindex(list, line);
		break;
	case 6:
		Myindex(list, line);
		break;
	case 7:
		Myclient_body_limit(list, line);
		break;
	case 8:
		Myerror_page(list, line);
		break;
	default:
		break;
	}
}

void	Server::seter(std::string str, int line){
	std::vector<std::string> list;
	int	token;

	list = splitString(str, " \t");
	if (list.empty() || list[0][0] == '#')
		return ;
	if (!isInMyList(list[0], token))
		throw std::invalid_argument(throwmessage(line, "invalid input."));
	if (list[list.size() - 1][list[list.size() - 1].length() - 1] != ';')
		throw std::invalid_argument(throwmessage(line, "the line have to end with ';'."));
	if (isWhitespace(str) && str.size() > 0)
		throw std::invalid_argument(throwmessage(line, "extra whitespaces."));
	int f = 0;
	for (size_t i = str.size() - 1; i > 0; i--){
		if (str[i] == ';')
			f++;
		if (f == 2)
			throw std::invalid_argument(throwmessage(line, "Error: More than one semicolon in the string."));
	}
	if (str[0] != '\t' || std::isspace(str[1]))
		throw std::invalid_argument(throwmessage(line, "execept one TAB at the start."));
	if (std::isspace(str[str.length() - 1]))
		throw std::invalid_argument(throwmessage(line, "extra whitespaces at the end."));
	for (size_t i = 1; i < str.length() - 1; ++i) {
        if (std::isspace(str[i]) && std::isspace(str[i + 1]))
			throw std::invalid_argument(throwmessage(line, "extra whitespaces."));
	}
	set_value(list, token, line);
}

void	Server::checkfirstline(std::string str, int line){
	if (std::isspace(str[0]) )
        throw std::invalid_argument(throwmessage(line, "extra whitespaces at the beginning."));
	if (std::isspace(str[str.length() - 1]))
		throw std::invalid_argument(throwmessage(line, "extra whitespaces at the end."));
	// Check for excessive whitespace between words
    for (size_t i = 0; i < str.length() - 1; ++i) {
        if (std::isspace(str[i]) && std::isspace(str[i + 1]))
			throw std::invalid_argument(throwmessage(line, "extra whitespaces."));
    }
	if (str.compare("server {"))
		throw std::invalid_argument(throwmessage(line, "invalid input."));
}

void	Server::checklastline(std::string str, int line, int firstline){
	if (str.empty())
		throw std::invalid_argument(throwmessage(firstline, "Error: Invalid Input, must close your server with bracket '}' ."));
	if (std::isspace(str[0]) )
        throw std::invalid_argument(throwmessage(line, "Error: extra whitespaces at the beginning."));
	if (std::isspace(str[str.length() - 1]))
		throw std::invalid_argument(throwmessage(line, "Error: extra whitespaces at the end."));
	// Check for excessive whitespace between words
    for (size_t i = 0; i < str.length() - 1; ++i) {
        if (std::isspace(str[i]) && std::isspace(str[i + 1]))
			throw std::invalid_argument(throwmessage(line, "Error: extra whitespaces."));
    }
	if (str.compare("}"))
		throw std::invalid_argument(throwmessage(line, "Error: Invalid Input"));
}	


void	Server::setmylocation(std::map<int, std::string>::const_iterator &it, std::map<int, std::string> &server){
	std::map<int, std::string> locations;
	std::map<int, std::string>::const_iterator itf = it;
	int itfirst = it->first;
	for(; it != server.end() && it->second.find(")") == std::string::npos; ++it){
		if (it != itf && it->second.find("(") != std::string::npos)
			throw std::invalid_argument(throwmessage(itfirst, "lalalalal"));
		locations[it->first] = it->second;
	}
	if (it->second.find(")") != std::string::npos)
		locations[it->first] = it->second;
	else if (it == server.end())
		throw std::invalid_argument(throwmessage(itfirst, "okokoko"));
	Location	local(locations);
	setLocations(local);
}

void	Server::parse(std::map<int, std::string> &server){
	std::map<int, std::string>::const_iterator it = server.begin();
	std::map<int, std::string>::reverse_iterator rit = server.rbegin();

	checkfirstline(it->second, it->first);
	checklastline(rit->second, rit->first, it->first);
	it++;
	for (; it != server.end(); it++)
    {
		if (it->second.find("}") == std::string::npos){
			if (it->second.find("(") != std::string::npos){
				setmylocation(it, server);
			}
			else
       			seter(it->second, it->first);
		}
	}
}





void	Server::Myserver_name(std::vector<std::string> list, int line){
	if (!list[list.size() - 1].compare(";"))
		list.pop_back();
	if (list.size() != 2 || list.empty())
		throw std::invalid_argument(throwmessage(line, "Error: Invalide Input in server_name."));
	this->server_name = withoutsemicolon(list[1]);
}



void	Server::Myroot(std::vector<std::string> list, int line){
	if (!list[list.size() - 1].compare(";"))
		list.pop_back();
	if (list.size() != 2 || list.empty())
			throw std::invalid_argument(throwmessage(line, "Error: Invalide root path."));
	this->root = withoutsemicolon(list[1]);
}

void	Server::Myindex(std::vector<std::string> list, int line){
	if (!list[list.size() - 1].compare(";"))
		list.pop_back();
	if (list.size() != 2 || list.empty())
			throw std::invalid_argument(throwmessage(line, "Error: Invalide index path."));
	this->index = withoutsemicolon(list[1]);
}

void	Server::Myclient_body_limit(std::vector<std::string> list, int line){
	if (!list[list.size() - 1].compare(";"))
		list.pop_back();
	if (list.size() != 2 || list.empty())
			throw std::invalid_argument(throwmessage(line, "Error: Invalide input in Client Body Limit."));
	char* endPtr;
    long parsedcbl = std::strtol(withoutsemicolon(list[1]).c_str(), &endPtr, 10);
	if (*endPtr != '\0' || parsedcbl < 0 || parsedcbl > static_cast<long>(INT_MAX))
		throw std::invalid_argument(throwmessage(line, "Error: Invalide input in Client Body Limit."));
    this->client_body_limit = static_cast<int>(parsedcbl);
}

void	Server::Myautoindex(std::vector<std::string> list, int line){
	std::string autoin;

	if (!list[list.size() - 1].compare(";"))
		list.pop_back();
	if (list.size() != 2 || list.empty())
		throw std::invalid_argument(throwmessage(line, "Error: Invalide Input in autoindex ON/OFF."));
	autoin = withoutsemicolon(list[1]);
	if (!autoin.compare("ON"))
		this->autoindex = 1;
	else if (!autoin.compare("OFF"))
		this->autoindex = 0;
	else
		throw std::invalid_argument(throwmessage(line, "Error: Invalide Input in autoindex ON/OFF."));
}

void	Server::Myallow_methods(std::vector<std::string> list, int line){
	std::vector<std::string> allows;
	std::string	method;

	if (!list[list.size() - 1].compare(";"))
		list.pop_back();
	if (list.size() == 1 || list.empty())
		throw std::invalid_argument(throwmessage(line, "Error: Invalide Input in allows methodes."));
	for (size_t i = 1; i < list.size(); i++)
	{
		method = withoutsemicolon(list[i]);
		if (method.compare("GET") && method.compare("POST") && method.compare("DELETE"))
			throw std::invalid_argument(throwmessage(line, "Error: Invalide Input in allows methodes."));
		allows.push_back(method);
	}
	if (allows.empty() || allows.size() > 3)
		throw std::invalid_argument(throwmessage(line, "Error: Invalide Input in allows methodes."));
	this->allow_methods = allows;
}

void	Server::Myerror_page(std::vector<std::string> list, int line){
	if (!list[list.size() - 1].compare(";"))
		list.pop_back();
	if (list.size() != 3 || list.empty())
		throw std::invalid_argument(throwmessage(line, "Error: Invalide Input in Error_Page."));
	char* endPtr;
    long errornbr = std::strtol(withoutsemicolon(list[1]).c_str(), &endPtr, 10);
	if (*endPtr != '\0' || errornbr < 0 || errornbr > static_cast<long>(INT_MAX))
		throw std::invalid_argument(throwmessage(line, "Error: Invalide Error Code."));
	std::string path = withoutsemicolon(list[2]);
	this->error_page[errornbr] = path;
}

void	Server::Mylocations(std::vector<Location>& sname){
	this->locations = sname;
}


//=============== seters ===================//

void	Server::setPort(std::string str){
	this->port = str;
}

void	Server::setHostname(std::string str){
	this->hostname = str;
}

void		Server::setServerName(std::string str){
	this->server_name = str;
}

void		Server::setRoot(std::string str){
	this->root = str;
}

void		Server::setIndex(std::string str){
	this->index = str;
}

void		Server::setClientBodyLimit(int nbr){
	this->client_body_limit = nbr;
}

void		Server::setAutoindex(bool b){
	this->autoindex = b;
}

void		Server::setAllowMethods(std::string method){
	this->allow_methods.push_back(method);
}

void		Server::setErrorPage(int error, std::string path){
	this->error_page[error] = path; 
}

void		Server::setLocations(Location& obj){
	this->locations.push_back(obj);
}


//=============== geters ===================//
std::string	Server::getIndex(void) const{
	return this->index;
}

std::string	Server::getRoot(void) const{
	return this->root;
}

std::string	Server::getServerName(void) const{
	return this->server_name;
}

std::string	Server::getPort(void) const{
	return this->port;
}

std::string	Server::getHostname(void) const{
	return this->hostname;
}

int	Server::getClientBodyLimit(void) const{
	return this->client_body_limit;
}

bool	Server::getAutoindex(void) const{
	return this->autoindex;
}

std::map<int, std::string>	Server::getErrorPage(void) const{
	return this->error_page;
}

std::vector< std::string>	Server::getAllowMethods(void) const{
	return this->allow_methods;
}

std::vector<Location>	Server::getLocations(void) const{
	return this->locations;
}