#include "../includes/irc.hpp"

int	irc(int ac, char **av) {
	if (ac == 3) {
		try {
			Server serv(av[2], av[1]);;
			serv.start();
		}
		catch (std::exception & ex) {
			rplDisplay(ex.what());
		}
	} else 
		rplDisplay("Must be called like : ./irc <port> <password>");
	return 0;
}	