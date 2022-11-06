#include "../includes/irc.hpp"

void	cmdOper(Server & serv, Client * client, std::vector<std::string> arg) {
	std::string rpl;
	if (arg.size() < 3) {
		rpl = ":math 461 " + client->getNick() + " OPER :Not enough parameters\r\n";
		rplDisplay(rpl);
		send(client->getFd(), rpl.c_str(), rpl.size(), 0);
		return ;
	}
	else if (arg[2] != serv.getIrcPass()) {
		rpl = ":math 464" + client->getNick() + " :Password incorrect\r\n";
		rplDisplay(rpl);
		send(client->getFd(), rpl.c_str(), rpl.size(), 0);
	}
	else if (!serv.findClientByName(arg[1])){
		rpl = ":math 999" + client->getNick() + " :No such nick\r\n";
		rplDisplay(rpl);
		send(client->getFd(), rpl.c_str(), rpl.size(), 0);
	}
	else {
		Client * op = serv.findClientByName(arg[1]);
		op->op = true;
		rpl = ":math 381 " + client->getNick() + " :You are now an IRC operator\r\n";
		rplDisplay(rpl);
		send(client->getFd(), rpl.c_str(), rpl.size(), 0);
	}
}


/*       381    RPL_YOUREOPER
              ":You are now an IRC operator"

		464    ERR_PASSWDMISMATCH
              ":Password incorrect"
*/