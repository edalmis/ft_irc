#include "../includes/irc.hpp"	

void	cmdTopic(Server & serv, Client * client, std::vector<std::string> arg) {
	std::string rpl;
	Channel * chan;
	if (arg.size() == 1) {
		/*
		? RPL_NEEDMOREPARAMS
		*/
		rpl = "461 TOPIC :Not enough parameters\r\n";
		rplDisplay(rpl);
		send(client->getFd(), rpl.c_str(), rpl.size(), 0);
	}
	else if (arg.size() == 2) {
		chan = serv.findChannelByName(arg[1]);
		if (!chan) {
			/* 
			* No such channel.
			*/
			rpl = "403 " + arg[1] + " :No such channel\r\n";
			rplDisplay(rpl);
			send(client->getFd(), rpl.c_str(), rpl.size(), 0);
		}
		else if (chan->getTopic().empty()) {
			/* 
			* RPL_NOTOPIC
			*/
			rpl = "332 " + client->getNick() + " " + chan->getName() + " :No Topic set\r\n";
			rplDisplay(rpl);
			send(client->getFd(), rpl.c_str(), rpl.size(), 0);
		}
		else if (chan->getTopic().empty() != 0) {
			/* 
			* RPL_TOPIC
			*/
			rpl = "332 " + client->getNick() + " " + chan->getName() + " " + chan->getTopic() + "\r\n";
			rplDisplay(rpl);
			send(client->getFd(), rpl.c_str(), rpl.size(), 0);
		}
	}
	else if (arg.size() >= 3) {
		chan = serv.findChannelByName(arg[1]);
		if (!chan) {
			/* 
			* No such channel.
			*/
			rpl = "403 " + arg[1] + " :No such channel\r\n";
			rplDisplay(rpl);
			send(client->getFd(), rpl.c_str(), rpl.size(), 0);
		}
		if (chan->isSuper(client)) {
			/* 
			! the client must be a superUser to set the topic.
			*/
			if (arg[2] == ":") {
				// Clear the Topic
				chan->clearTopic();
			}
			else {
				/*
				* Replace the current topic by the string [Topic].
				*/
				std::string topic;
				for (size_t i = 2; i < arg.size(); i++) {
					topic += arg[i];
					if (i != arg.size())
						topic += " ";
				}
				chan->setTopic(topic);
				rpl = ":" + client->getNick() + "!" + client->getUser() + "@127.0.0.1 TOPIC " + chan->getName() + " " + topic;
				rplDisplay(rpl);
				send(client->getFd(), rpl.c_str(), rpl.size(), 0);
			}
		}
		else {
			/*
			! The client isn't a superUser.
			*/
			rpl = "482 " + chan->getName() + " :You're not channel operator\r\n";
			rplDisplay(rpl);
			send(client->getFd(), rpl.c_str(), rpl.size(), 0);
		}
	}
}