#pragma once

# include <iostream>
# include <exception>
# include <cstdlib>
# include <string>
# include <vector>
# include <stdio.h>
# include <netdb.h>
# include <netinet/in.h>
# include <stdlib.h>
# include <string.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <stdio.h> 
# include <string.h>		//strlen 
# include <stdlib.h> 
# include <errno.h> 
# include <unistd.h>   		//close 
# include <arpa/inet.h>		//close 
# include <sys/types.h> 
# include <sys/socket.h> 
# include <netinet/in.h> 
# include <sys/time.h>		//FD_SET, FD_ISSET, FD_ZERO macros 

# include "except.hpp"

/* Utils */
std::vector<std::string>	split (std::string s, std::string delimiter);
void						colorMsg(std::string colorCode, char *msg);
void						rplDisplay(std::string rpl);

class	Server;
class	Client;
class	Server;

void	cmdMode(Server & serv, Client * client, std::vector<std::string> line);
void	cmdPass(std::string pass, Server & serv, Client * client);
void	cmdNick(Server & serv, Client * client, std::string nick);
void	cmdUser(Server & serv, Client * client, std::vector<std::string> line);
void	cmdJoin(Server & serv, Client * client, std::vector<std::string> cmd);
void	cmdPrivMsg(Server & serv, Client * client, std::vector<std::string> line);
void	cmdPart(Server & serv, Client * client, std::vector<std::string> cmd);
void	cmdTopic(Server & serv, Client * client, std::vector<std::string> arg);
void	cmdOper(Server & serv, Client * client, std::vector<std::string> arg);
void	cmdKill(Server & serv, Client * client, std::vector<std::string> arg);
void	cmdKick(Server & serv, Client * client, std::vector<std::string> line);
void	cmdQuit(Server & serv, Client * client, std::vector<std::string> arg);
void	cmdNames(Server & serv, Client* client, std::vector<std::string> line);

# include "Client.hpp"
# include "Channel.hpp"
# include "Server.hpp"
# include "parse.hpp"

int		irc(int ac, char **av);
void 	tcp(Server serv);
void	Server_loop(Server serv, fd_set readfds, int max_clients, struct sockaddr_in address, int addrlen, int i, int *client_socket);

/* Parser */
void	parser_cmd(std::string cmd, int s, Server serv);
void	is_Accepted(Client * client);

/*  Channel Interaction */
void	sendToChannel(std::string msg, Channel * chan, Client * client, bool himself);
void	sendChannelInfo(Channel * chan, Client * client);