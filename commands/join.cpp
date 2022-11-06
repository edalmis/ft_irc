#include "../includes/irc.hpp"


/*  Command: JOIN
   Parameters: ( <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] ) / "0"
*/

void	sendChannelInfo(Channel * chan, Client * client) {
	std::string chanInfo;

	/* RPL_TOPIC */
	chanInfo = "332 " + client->getNick() + " " + chan->getName() + " :No Topic set\r\n";
	send(client->getFd(), chanInfo.c_str(), chanInfo.size(), 0);
	chanInfo.clear();


	/* RPL_NAMREPLY */
	chanInfo = "353 " + client->getNick() + " = " + chan->getName() + " :";
	for (size_t i = 0; i < chan->members.size(); i++) {
		if (chan->isSuper(chan->members[i]) == true)
			chanInfo += "@";
		chanInfo += chan->members[i]->getNick() + " ";
	}
	chanInfo.erase(chanInfo.length() - 1);
	chanInfo += "\r\n";
	send(client->getFd(), chanInfo.c_str(), chanInfo.size(), 0);
	chanInfo.clear();


	/* RPL_ENDOFNAMES */
	chanInfo = "366 " + client->getNick() + " " + chan->getName() + " :End of NAMES ist\r\n";
	send(client->getFd(), chanInfo.c_str(), chanInfo.size(), 0);
	return ;
}

void	cmdJoin(Server & serv, Client * client, std::vector<std::string> cmd) {
	std::string errMsg;
	if (cmd.size() == 1) {
		/* If there is only JOIN it miss some parameters */
		errMsg = "461 JOIN :Not enough parameters\r\n";
		rplDisplay(errMsg);
		send(client->getFd(), errMsg.c_str(), errMsg.size(), 0);
		return ;
	}
	else if (cmd.size() == 2 && cmd[1][0] == '0')
	{
		/* In the RFC they that the '0' parameters must make the client leave all the channel where he is
		   but I don't know if we have to do it, so just in case I've made it */
		for (size_t i = 0; i < serv.Chan.size();  i++)
			serv.delClientFromChann(client, serv.Chan[i], serv);
		client->setCurrChannelNull();
		return;
	}
	else if (cmd[1][0] != '#' && cmd[1][0] != '&') {
		/* When you want to join a channel the name of the channel must start with a # or & */
		errMsg = "476 JOIN :Bad Channel Mask\r\n";
		rplDisplay(errMsg);
		send(client->getFd(), errMsg.c_str(), errMsg.size(), 0);
		return ;
	}
	

	/* The third argument of the command JOIN is the key (password of the Channel).
	   Here I'll try to get it */
	
	bool setK = false;
	std::vector<std::string>	keys;
	size_t	sizeK = 0;
	if (cmd.size() == 3){
		std::string d(",");
		keys  = split(cmd[2], d);
		setK = true;
		sizeK = keys.size();
	}

	/* Here I split the arg with ',' because you can join multiples channels with JOIN like :
	   JOIN #first,#second */
	std::string delim = ",";
	std::vector<std::string> newChann = split(cmd[1], delim);

	std::string _Dis;
	for (size_t i = 0; i < newChann.size(); i++) {
		_Dis = "Join Channel name is " + newChann[i] + "\n";
		colorMsg("\e[4;33m", (char *)_Dis.c_str());
	}
	/* _wtj means want to join, here we gonna search if the channel exist in the vector 
	   of channel that we have in the serv, and if it does not exist,
	   we will have to create one */
	for (size_t i = 0; i < newChann.size(); i++) {
		Channel* _wtj = serv.findChannelByName(newChann[i]);
		if (!_wtj) {
			/* If _wtj is NULL it means that there is no Channel with this name. 
			So here I create a Channel and push it back to the vector of channel that the server possess */
			Channel  *to_add = new Channel(newChann[i]);
			to_add->addSuper(client);
			to_add->addMembers(client);
			if (setK == true && i < keys.size() - 1) {
				std::string keyInfo = "Set " + keys[i] + " as pass for the Channel : " + to_add->getName();
				colorMsg("\e[4;34m", (char *)keyInfo.c_str());
				to_add->setPass(keys[i]);
			}
			serv.Chan.push_back(to_add);
			client->setCurrChannel(to_add);
			sendChannelInfo(to_add, client);
			if (newChann.size() == 1)
				return ;
			std::cout << "Size of chann : " << serv.Chan.size() << std::endl;
			/* There is a specific message to send to all the users of the channel that the client join */
			// std::string wlcMsg;
			// wlcMsg = client->getNick() + "!" + client->getUser() + "@127.0.0.1 JOIN " + newChann[i] + "\r\n";
			// sendToChannel(wlcMsg, _wtj, client, true); 
		}
		else {
			/* If it's not NULL it means a channel with this name already exists. 
			So we have to make this client join this channel */
			if (_wtj->passState() == true) {
				if (sizeK == 0) {
					rplDisplay("No key precised");
					errMsg = ": 475 " + client->getNick() + " " + _wtj->getName() + " :Cannot join channel (+k)\r\n";
					rplDisplay(errMsg);
					send(client->getFd(), errMsg.c_str(), errMsg.size(), 0);
				}
				else if (sizeK != 0 && i < sizeK - 1) {
					std::string keyInfo = "Client has send key as : " + keys[i] + "\n";
					colorMsg("\e[4;35m", (char *)keyInfo.c_str());
					if (keys[i] != _wtj->getPass()) {
						std::string keyInfo = "Client has sent the wrong key\n";
						colorMsg("\e[4;35m", (char *)keyInfo.c_str());
						errMsg = "475 " + client->getNick() + " " + _wtj->getName() + " :Cannot join channel (+k)\r\n";
						rplDisplay(errMsg);
						send(client->getFd(), errMsg.c_str(), errMsg.size(), 0);
					}
					else if (keys[i] == _wtj->getPass()) {
						std::string keyInfo = "Client has sent the right key\n";
						colorMsg("\e[4;35m", (char *)keyInfo.c_str());
						_wtj->members.push_back(client);
						client->setCurrChannel(_wtj);

						/* There is a specific message to send to all the users of the channel that the client join */
						std::string wlcMsg;
						wlcMsg = ":irc " + client->getNick() + "!" + client->getUser() + "@127.0.0.1 JOIN " + newChann[i] + "\r\n";
						
						sendToChannel(wlcMsg, _wtj, client, true);
						sendChannelInfo(_wtj, client);
					}
				}
			}
			else {
				rplDisplay("No key");
				_wtj->members.push_back(client);
				client->setCurrChannel(_wtj);

				/* There is a specific message to send to all the users of the channel that the client join */
				std::string wlcMsg;
				wlcMsg = ":irc " + client->getNick() + "!" + client->getUser() + "@127.0.0.1 JOIN :" + newChann[i] + "\r\n";
				rplDisplay(wlcMsg);
				sendToChannel(wlcMsg, _wtj, client, true); 
				sendChannelInfo(_wtj, client);
			}
		}
	}
	if (client->getCurr())
		std::cout << "This user belong to the channel : " << client->getCurr()->getName() << std::endl;
	return ;
}