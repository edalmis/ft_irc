#include "../includes/irc.hpp"

void	cmdNames(Server & serv, Client* client, std::vector<std::string> line)
{
	std::string	msg;
	Channel*	curr_chan = NULL;
	if (line.size() < 2)
	{
        msg = "461 " + client->getNick() + "Not enough parameters\r\n";
		rplDisplay(msg);
        send(client->getFd(), msg.c_str(), strlen(msg.c_str()), 0);
        return;
    }
	else if (line.size() == 2)
	{
		curr_chan = serv.findChannelByName(line[1]);
		if (curr_chan == NULL)
        {
            msg = ":" + client->getNick() + "!" + client->getUser() + "@127.0.0.1 " + line[1] + " :No such channel\r\n";
            rplDisplay(msg);
            send(client->getFd(), msg.c_str() , msg.length(), 0);
            return ;
        }
        else {
            if (curr_chan->isHere(client) == true)
                sendChannelInfo(curr_chan, client);
            else {
                msg = "442 " + client->getNick() + " " + curr_chan->getName() + " :You're not on that channel\r\n";
                rplDisplay(msg);
                send(client->getFd(), msg.c_str(), msg.size(), 0);
            }
        }
	}
}