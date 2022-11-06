#include "../includes/irc.hpp"


void	cmdPrivMsg(Server & serv, Client * client, std::vector<std::string> line) {
	std::string errMsg;

	std::cout << "Server to send PIRVMSG = " << line[1] << std::endl;
	/* If there is a # or a & it means a channel, otherwise it means a user */
	if (line[1].find("#") == 0 || line[1].find("&") == 0)
	{
		std::string Chann(line[1]);
		Channel *to_send = serv.findChannelByName(Chann);
		std::cout << "Want to chat in the " << to_send->getName() << " channel" << std::endl;
		bool testClient = serv.clientExist(line[1]);
		std::cout << testClient << std::endl;
		if (!to_send && testClient == false) {
			/* If the channel where the client want to send his message does not exists */
			errMsg = "404 " + client->getNick() + " " + line[1] + " :No such nick/channel\r\n";
			send(client->getFd(), errMsg.c_str(), errMsg.size(), 0);
			std::cout << errMsg << std::endl;
			return ;
		}
		else if (to_send && to_send->isHere(client) == true) {
			std::string msg = ":" + client->getNick() + "!" + client->getUser() + "@127.0.0.1 PRIVMSG " + to_send->getName() + " :";

			if (line[2][1] == ':')
				msg += &line[2][1]; // Message
			else
				msg += line[2];
			
			if (line.size() > 3)
				msg += " ";
			for (size_t i = 3; i < line.size(); i++) {
				msg += line[i];
				if (line.size() > i)
					msg += " ";
			}
			msg += "\r\n";
			std::cout << "Prototype of what I send to the client : " << msg << std::endl;
			sendToChannel(msg, to_send, client, false);
			return ;
		}
		else {
			errMsg = "404 " + client->getNick() + " " + to_send->getName() + "  :Cannot send to channel\r\n";
			std::cout << errMsg << std::endl;
			send(client->getFd(), errMsg.c_str(), errMsg.size(), 0);
		}
	}
	else {
		bool exist = serv.clientExist(line[1]);
		if (exist == false) {
			errMsg = "401 " + client->getNick() + " " + line[1] + " :No such nick/channel\r\n";
			send(client->getFd(), errMsg.c_str(), errMsg.size(), 0);
			std::cout << errMsg << std::endl;
			return ;
		}
		else {
			std::string msg = ":" + client->getNick() + "!" + client->getUser() + "@127.0.0.1 PRIVMSG " + line[1] + " :";

			msg += &line[2][1]; // Message
			if (line.size() > 3)
				msg += " ";
			for (size_t i = 3; i < line.size(); i++) {
				msg += line[i];
				if (line.size() > i)
					msg += " ";
			}
			msg += "\r\n";
			send(serv.findClientByName(line[1])->getFd(), msg.c_str(), msg.size(), 0);
			std::cout << errMsg << std::endl;
			return ;
		}
	}
}