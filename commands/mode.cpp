#include "../includes/irc.hpp"

// char detMode(std::string mode) {
// 	char arg[7] = {'a', 'i', 'w', 'r', 'o', 'O', 's'};
	
// 	return 'X';
// }

#define VE_NMP "461"
#define VERR_NEEDMOREPARAMS " :Not enough parameters\r\n"

#define VE_UDM "502"
#define VERR_USERSDONTMATCH " :cannot change mode for other users\r\n"

#define VE_MUF "501"
#define VERR_UMODEUNKNOWNFLAG " :Unknown MODE flag\r\n"

void	cmdMode(Server & serv, Client * client, std::vector<std::string> line) {
	// std::string errMsg;

	// if (line.size() == 2 || line.size() == 1) {
	// 	std::string E_NMP(VE_NMP);
	// 	std::string ERR_NEEDMOREPARAMS(VERR_NEEDMOREPARAMS);
	// 	errMsg = E_NMP + " MODE " + ERR_NEEDMOREPARAMS;
	// 	send(client->getFd(), errMsg.c_str(), strlen(errMsg.c_str()), 0);
	// 	return ;
	// }
	// else if (line[1] != client->getNick()) {
	// 	std::string E_UDM(VE_UDM);
	// 	std::string ERR_USERSDONTMATCH(VERR_USERSDONTMATCH);
	// 	errMsg = E_UDM + ERR_USERSDONTMATCH;
	// 	send(client->getFd(), errMsg.c_str(), strlen(errMsg.c_str()), 0);
	// 	return ;
	// }
	// else if ){
	// 	char mode = detMode(line[2]);
	// 	if (mode == 'X') {

	// 	}
		
	// }
	(void)serv;
	// If the client tries to set the mode for the Client like MODE Mathieu +i
	// We do not have to handle this mode command for the client.
	if (line[1][0] != '#' && line[1][0] != '&') {
		std::string errMsg;
		errMsg = "666 " + client->getNick() + " :We do not handle client modes !\r\n";
		// std::cout << "This is what I send for the mode command : " << errMsg << std::endl;
		rplDisplay(errMsg);
		send(client->getFd(), errMsg.c_str(), strlen(errMsg.c_str()), 0);
	}
	return ;
}