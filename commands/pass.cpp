#include "../includes/irc.hpp"

void	cmdPass(std::string pass, Server & serv, Client * client) {
	if (client->accepted == true)
		return ;
	else if (pass == serv.getPassword()) {
		client->setPwd(pass);
		client->pass = true;
		std::cout << "\e[1;36mPassword set\e[0m" << std::endl;
	}
	else if (pass != serv.getPassword() && client->pass == true) {
		std::string rpl;
		rpl = "462 :Unauthorized command (already registered)\r\n";
		rplDisplay(rpl);
		send(client->getFd(), rpl.c_str(), rpl.size(), 0);
	}
	is_Accepted(client);
	return ;
}