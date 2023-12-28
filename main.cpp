#include <iostream>
#include "Config.hpp"

int	main(int ac, char **av){
	try
	{
		if (ac != 2)
			throw std::invalid_argument("server: wrong argument");
		Config	config(av[1]);

	std::vector<Server> srv = config.Servers;
	for (size_t i = 0; i < srv.size(); i++)
    {
		std::cout << "----------------------" <<std::endl;
		std::cout << "SERVER:\t"<< i + 1 <<std::endl;
        std::cout << "server name:\t["<< srv[i].getServerName() << "]" << std::endl;
        std::cout << "port:\t\t["<< srv[i].getPort() << "]" << std::endl;
        std::cout << "hostname:\t["<<  srv[i].getHostname() << "]" << std::endl;
        std::cout << "root path:\t["<<  srv[i].getRoot() << "]" << std::endl;
        std::cout << "autoindedx:\t["<<  srv[i].getAutoindex() << "]" << std::endl;
        std::cout << "indedx:\t\t["<<  srv[i].getIndex() << "]" << std::endl;
        std::cout << "CBL:\t\t["<<  srv[i].getClientBodyLimit() << "]" << std::endl;
		const std::map<int, std::string>& errorPageMap = srv[i].getErrorPage();
		for (std::map<int, std::string>::const_iterator it = errorPageMap.begin(); it != errorPageMap.end(); ++it){
        	std::cout << "Error_page:\t["<<  it->first << "]"<< " [" << it->second << "]" << std::endl;
		}
		std::cout << "Allow Methods:\t[ " ;
		const std::vector<std::string>& allows = srv[i].getAllowMethods();
		for (size_t i = 0; i < allows.size(); i++){
        	std::cout << allows[i]<< " ";
		}
		std::cout << "]"<<std::endl;
		std::vector<Location> local = srv[i].getLocations();
		for (size_t i = 0; i < local.size(); i++){
			std::cout << "Local: " << i + 1 << std::endl;
			std::cout << "\tpath:\t\t[" << local[i].getPath() << "]" << std::endl;
			std::cout << "\troot:\t\t[" << local[i].getRoot() << "]" << std::endl;
			std::cout << "\tindex:\t\t[" << local[i].getIndex() << "]" << std::endl;
			std::cout << "\tautoindex:\t[" << local[i].getAutoindex() << "]" << std::endl;
			std::cout << "\tAllow Methods:\t[ " ;
			const std::vector<std::string>& allowss = local[i].getAllowMethods();
			for (size_t i = 0; i < allowss.size(); i++){
        		std::cout << allowss[i]<< " ";
			}
			std::cout << "]"<<std::endl;
		}

    }
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}