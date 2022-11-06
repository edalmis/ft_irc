#include "../includes/irc.hpp"

void	cmdPart(Server & serv, Client * client, std::vector<std::string> cmd) {
	std::string errMsg;

	/* If there is just only the commad PART it miss the name of the channel to leave */
	if (cmd.size() == 1) {
		errMsg = "461 PART :Not enough parameters\r\n";
		send(client->getFd(), errMsg.c_str(), errMsg.size(), 0);
		rplDisplay(errMsg);
		return ;
	}

	/* I split with the delim ',' because you can leave multiples channel in one command like this : 
	   PART #FIRST,#SECOND */
	std::string delim(",");
	std::vector<std::string> Chan_to_leave = split(cmd[1], delim);


	for (size_t i = 0; i < Chan_to_leave.size(); i++) {
		std::string Chann = Chan_to_leave[i];
		std::string INFO = "Channel to leave : " + Chann;
		rplDisplay(INFO);
		Channel * to_leave = serv.findChannelByName(Chann);
		if (!to_leave) {
			/* If the channel the client wants to leave doesn't exist */
			errMsg = "403 " + client->getNick() + " " + Chann + " :No such channel\r\n";
			rplDisplay(errMsg);
			send(client->getFd(), errMsg.c_str(), errMsg.size(), 0);
		}
		else {
			/* If the client belong to the channel I leave it */
			if (to_leave->isMembers(client) == true || to_leave->isSuper(client) == true) {
				errMsg = ":" + client->getNick() + "!" + client->getUser() + "@127.0.0.1 PART";
				errMsg += " :" + Chann + " ";
				if (cmd.size() > 2) {
					/* If a the client leave with a message */
					std::string partMsg;
					for (size_t i = 2; i < cmd.size(); i++) {
						errMsg += cmd[i];
						if (i != cmd.size() - 1)
							errMsg += " ";
					}
				}
				errMsg += "\r\n";
				rplDisplay(errMsg);
				/* Send the information that the client leave the channel to all the other user that belongs to the CHANNEl */
				client->setCurrChannelNull();
				sendToChannel(errMsg, to_leave, client, true);
				to_leave->deleteClient(client);
			}
			else {
				/* Otherwise I send to the client an error message */ 
				errMsg = "442 " + Chann + " :You're not on that channel\r\n";
				rplDisplay(errMsg);
				send(client->getFd(), errMsg.c_str(), errMsg.size(), 0);
			}
		}
	}
}
