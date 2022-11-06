#include "../includes/irc.hpp"

void	cmdKill(Server & serv, Client * client, std::vector<std::string> arg) {
	std::string rpl;
	if (arg.size() < 2) {
		rpl = ":math 461 " + client->getNick() + " KILL :Not enough parameters\r\n";
		send(client->getFd(), rpl.c_str(), rpl.size(), 0);
		rplDisplay(rpl);
		return ;
	}
	Client * cl = serv.findClientByName(arg[1]);
	if (!cl) {
		rpl = ":math 401 " + client->getNick() + " " + arg[1] + "  :No such nick/channel\r\n";
		send(client->getFd(), rpl.c_str(), rpl.size(), 0);
		rplDisplay(rpl);
		return ;
	}
	else if (cl == client) {
		rpl = ":math 401 " + client->getNick() + " " + arg[1] + "  :You can't kill yourself\r\n";
		send(client->getFd(), rpl.c_str(), rpl.size(), 0);
		rplDisplay(rpl);
		return ;
	}
	else if (client->op == false) {
		rpl = ":math 481 " + client->getNick() + " :Permission Denied- You're not an IRC operator\r\n";
		send(client->getFd(), rpl.c_str(), rpl.size(), 0);
		rplDisplay(rpl);
		return ;
	}
	else {
		rpl = ":" + client->getNick() + "!" + client->getUser() + "127.0.0.1 KILL : ";
		if (arg.size() == 3 && arg[2] != ":")
			rpl += arg[2];
		rpl += "\r\n";
		rplDisplay(rpl);
		send(cl->getFd(), rpl.c_str(), rpl.size(), 0);
		for (size_t i = 0; i < serv.Chan.size(); i++)
			if (serv.Chan[i]->isMembers(cl)) {
				std::vector<std::string> chan_name;
				chan_name.push_back(serv.Chan[i]->getName());
				cmdPart(serv, cl, chan_name);
			}
		serv.deleteClient(cl->getFd());
		close(cl->getFd());
	}
}

/* 401    ERR_NOSUCHNICK
              "<nickname> :No such nick/channel"

*/