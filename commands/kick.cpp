#include "../includes/irc.hpp"

void	cmdKick(Server & serv, Client * client, std::vector<std::string> line) {
	
	std::string error;
    if (line.size() < 4)
    {
        error = "461 " + client->getNick() + "Not enough parameters\r\n";
		rplDisplay(error);
        send(client->getFd(), error.c_str(), strlen(error.c_str()), 0);
        return;
    }
    else if(line[1][0] != '#' && line[1][0] != '&')
    {
        error = "476 " + client->getNick() + ":Bad Channel Mask\r\n";
		rplDisplay(error);
        send(client->getFd(), error.c_str(), strlen(error.c_str()), 0);
        return;
    }
    else if(serv.findChannelByName(line[1]) == NULL)
    {
		error = "403 " + client->getNick() + ":No such channel\r\n";
		rplDisplay(error);
		send(client->getFd(), error.c_str(), strlen(error.c_str()), 0);
		return;
	}
	else if (serv.findChannelByName(line[1])->isSuper(client) == false){
        error = "482 " + client->getNick() + " :You're not channel operator\r\n";
		rplDisplay(error);
        send(client->getFd(), error.c_str(), strlen(error.c_str()), 0);
        return;
	}
    else if (serv.findClientByName(line[2]) == NULL){
        error = "401 " + client->getNick() + ":No such nick/channel\r\n";
		rplDisplay(error);
        send(client->getFd(), error.c_str(), strlen(error.c_str()), 0);
        return;
	}
	else if (client->getCurr()!= NULL && serv.findClientByName(line[2])->getCurr() != client->getCurr()){
        error = "441 " + client->getNick() + " :They aren't on that channel\r\n";
		rplDisplay(error);
        send(client->getFd(), error.c_str(), strlen(error.c_str()), 0);
        return;
	}
	else
	{
		error = ":" + client->getNick() + "!" + client->getUser() + "@127.0.0.1 KICK " + line[1] + " " + line[2];
		if(line.size() > 4)
		{
			std::string k;
			for(size_t a = 3; a < line.size(); a++)
			{	
				k += line[a];
                k += " ";
			}
			error = "KICK " + line[1] + " " + line[2] + " " + k;
		}
		rplDisplay(error);
		send(client->getFd(), error.c_str(), error.length(), 0);
		Channel * chane = serv.findChannelByName(line[1]);
		Client * del = serv.findClientByName(line[2]);
		if (del->getCurr() == chane)
			del->setCurrChannelNull();
		chane->deleteMembers(del);
		// serv.findChannelByName(line[1])->deleteMembers(serv.findClientByName(line[2]));
		// serv.findClientByName(line[2])->setCurrChannelNull();
			//if(serv.findChannelByName(line[1])->isSuper(serv.findClientByName(line[2]))
	}
}