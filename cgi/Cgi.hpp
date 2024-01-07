#ifndef CGI_HPP
# define CGI_HPP

#include <iostream>
#include <vector>
#include <map>

class Cgi
{
private:
	char	**env;
	std::map<std::string, std::string> _env;
public:
	Cgi(/* args */);
	~Cgi();
	void	init();
};

#endif