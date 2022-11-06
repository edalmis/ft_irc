#pragma once

#include "irc.hpp"
#include "Server.hpp"
#include "Client.hpp"

class Server;
class Client;

class parse {
	private:
		std::string	currMess;
		std::vector<std::string>	tab;
		


	public:
		parse() : currMess(NULL) { }

		parse(char *buffer) : currMess(buffer) {
			// std::cout << "test" << std::endl;
			std::string delim("\r\n");
			tab = split(currMess, delim);
			if (tab.size() == 1) {
				delim = "\n";
				tab = split(currMess, delim);
			}
			// std::cout << tab.size() << std::endl;
			for (size_t i = 0; i < tab.size(); i++) {
				if (tab[i].empty() == 0)
					std::cout << "\e[0;33m[MESSSAGE][" << i << "]\e[0m = " << "\e[0;32m{" << tab[i] << "}\e[0m" << std::endl;
			}
		}

		void	to_register(Server & serv, Client * Client) {
			std::string delim(" ");
			for (size_t i = 0; i < tab.size(); i++) {
				std::vector<std::string> line = split(tab[i], delim);
				if (line[0] == "NICK" && line.size() == 2)
					cmdNick(serv, Client, line[1]);
				else if (line[0] == "NICK" && line.size() == 1) {
					std::string to_send = "431 " + Client->getNick() + " :No nickname given\r\n";
					rplDisplay(to_send);
					send(Client->getFd(), to_send.c_str(), strlen(to_send.c_str()), 0);
				}	
				else if (line[0] == "USER" && line.size() >= 5)
					cmdUser(serv, Client, line);
				else if (line[0] == "USER" && line.size() <= 5) {
					std::string to_send = "461 USER :Not enough parameters\r\n";
					rplDisplay(to_send);
					send(Client->getFd(), to_send.c_str(), strlen(to_send.c_str()), 0); 
				}
				else if (line[0] == "PASS" && line.size() < 2) {
					std::string rpl;
					rpl = "461 PASS :Not enough parameters\r\n";
					rplDisplay(rpl);
					send(Client->getFd(), rpl.c_str(), rpl.size(), 0);
				}
				else if (line[0] == "PASS")
					cmdPass(line[1], serv, Client);
				/* If the client is accepted he can execute this commands otherwise he can't*/
				if (Client->accepted == true) {
					if (line[0] == "PING" && line.size() == 2) {
						std::string	to_send = ":irc PONG :" + line[1] + "\r\n";
						// colorMsg("\e[1;32m", (char *)to_send.c_str());
						rplDisplay(to_send);
						send(Client->getFd(), to_send.c_str(), strlen(to_send.c_str()), 0); 
					}
					else if (line[0] == "MODE")
						cmdMode(serv, Client, line);
					else if (line[0] == "JOIN")
						cmdJoin(serv, Client, line);
					else if (line[0] == "PRIVMSG" || line[0] == "NOTICE")
						cmdPrivMsg(serv, Client, line);
					else if (line[0] == "PART")
						cmdPart(serv, Client, line);
					else if (line[0] == "TOPIC")
						cmdTopic(serv, Client, line);
					else if (line[0] == "OPER")
						cmdOper(serv, Client, line);
					else if (line[0] == "kill")
						cmdKill(serv, Client, line);
					else if (line[0] == "KICK")
						cmdKick(serv, Client, line);
					else if (line[0] == "NAMES")
						cmdNames(serv, Client, line);
				//	else if (line[0] == "NAMES")
				//		cmdNames(Client);
				}
			}
		}	
};