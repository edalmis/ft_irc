#include "../includes/irc.hpp"

#define N_ERROR " :Nickname is already in use"

void	cmdNick(Server & serv, Client * client, std::string nick) {
	for (size_t i = 0; i < serv.user.size(); i++) {
		if (serv.user[i]->getNick() == nick) {
			std::string ERR;
			ERR = "433 * " + nick + N_ERROR + "\r\n";
			send(client->getFd(), ERR.c_str(), strlen(ERR.c_str()), 0);
			rplDisplay(ERR);
			return ;
		}
	}
	client->setNick(nick);
	client->nick = true;
	std::cout << "\e[1;36mNickname set\e[0m" << std::endl;
	is_Accepted(client);
	return ;
}