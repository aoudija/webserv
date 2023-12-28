#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include "Server.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

class Server;

class Config {
        Config();
        std::string         path_name;

        bool isWhitespace(const std::string& str);
        std::string throwmessage(int number, const std::string& str);
    public:
        std::vector<Server> Servers;
        Config(std::string av);
        ~Config();

        void    parse();
        void    parse_server(std::ifstream &ConfFile);
        bool    check_ext();
};

#endif // CONFIG_HPP
