#pragma once 

# include "irc.hpp"

#define TRUE   1 
#define FALSE  0 
#define SA struct sockaddr
#define MAX 80

class ErrorInPort : public std::exception {
	public:
		virtual const char* what() const throw()	
		{
			return ("Not a valable number for the Port between ]0; 65,535]");
		}
};

class ErrorInPortInput : public std::exception {
	public:
		virtual const char* what() const throw()	
		{
			return ("Port must contain only digit");
		}
};

#include "parse.hpp"
class parse;

class	Server {
	private:
		std::string 			_password;
		int						_port;
		std::string				_irc_pass;
	public:
		std::vector<Client *>	user;
		std::vector<Channel *>	Chan;
		int						sfd;
		int						fresh_fd;
		std::string				histo;
	public:
		Server() { } 

		Server(char *pwd, char *New_p) : _password(pwd) {
			std::string parse(New_p);

			for (size_t i = 0; i < parse.length(); i++) {
				if (!isdigit(parse[i]))
					throw ErrorInPortInput();
			}
			_port = atoi(New_p);
			if (_port < 0 || _port > 65535 || _port == 0)
				throw ErrorInPort();
			std::cout << "\e[0;35mChoose your operator IRC password : \e[0m";
			std::cin >> _irc_pass;
		}

		Server(const Server & rhs) {
			_port = rhs.getPort();
			_password = rhs.getPassword();
		}
		
		~Server() { }

		std::string getPassword(void) const {
			return (_password);
		}

		int	getPort(void) const {
			return (_port);
		}

		void	setPort(int port) {
			_port = port;
		}

		void	setPassword(std::string pass) {
			_password = pass;
		}

		std::string getIrcPass(void) {
			return (_irc_pass);
		}

		void	addClient(void) {
			struct sockaddr_in address;
			int new_socket;
			int addrlen = sizeof(address);

			Client	*tmp = new Client;
            if ((new_socket = accept(sfd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)  
				throw AcceptFailed();
			else
				tmp->setFd(new_socket);
			this->user.push_back(tmp);
			printf("\e[0;34m[+] New connection , socket fd is %d , ip is : %s , port : %d\n\e[0m" , new_socket , inet_ntoa(address.sin_addr) , ntohs
                   (address.sin_port));
			fresh_fd = new_socket;
		}

		void	deleteClient(unsigned long s) {
			/* When a client leave it has to leave all the channel where he's present */
			Client * client = findClientByFd(s);
			for (size_t i = 0; i < Chan.size(); i++) {
				if (Chan[i]->isHere(client)) {
					/* Here I set up all the arguments the part function needs to work */
					std::vector<std::string> arg;
					arg.push_back("PART");
					arg.push_back(Chan[i]->getName());
					cmdPart(*this, client, arg);
				}
			}

			std::vector<Client *>::iterator	it = user.begin();
			/* Now I delete the client from the vector of user the server possess */
			for (size_t i = 0; i < user.size() && user[i]->getFd() != s; i++, it++)
				;
			user.erase(it);
		}

		void	start(void) {
			int opt = TRUE;  
   			int addrlen;  
    		struct sockaddr_in address;  
				
			//set of socket descriptors 
			fd_set readfds;  
										
			//create a master socket 
			if( (sfd = socket(AF_INET , SOCK_STREAM , 0)) == 0)  
				throw SocketCreaFailed();
			
			/* set master socket to allow multiple connections , 
			this is just a good habit, it will work without this */
			if( setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )  
				throw SetSocketOptFailed();
			
			/* type of socket created */
			address.sin_family = AF_INET;  
			address.sin_addr.s_addr = INADDR_ANY;  
			address.sin_port = htons(getPort());  
				
			/* bind the socket to localhost port 8888 */
			if (bind(sfd, (struct sockaddr *)&address, sizeof(address))<0)  
				throw SocketBindFailed();

			printf("\e[0;34m[=] Listener on port %d \n\e[0m", getPort());  
				
			/* try to specify maximum of 3 pending connections for the master socket */
			if (listen(sfd, 3) < 0)  
				throw SocketListenFailed();
				
			/* accept the incoming connection */ 
			addrlen = sizeof(address);  
			puts("\e[0;34m[..] Waiting for connections ...\e[0m");  
		
			/* FIN TCP */

			char buffer[1025];
			int valread;
			int max_sd;
			int sd;
			int activity;
			while(42)  
			{  
				FD_ZERO(&readfds);  
				FD_SET(sfd, &readfds);  
				max_sd = sfd;  

				/* Set the FD with the function */
				for (size_t i = 0; i < user.size(); i++) {
					sd = user[i]->getFd();
					if (sd > 0)
						FD_SET(sd, &readfds);
					if (sd > max_sd)
						max_sd = sd;
				}
				/* select wait for an event */
				activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
				if ((activity < 0) && (errno!=EINTR))  
					throw SelectFailed();
				if (FD_ISSET(sfd, &readfds))  
					addClient();
				for (size_t i = 0; i < user.size(); i++) {
					/* get the fd of the client */
					sd = user[i]->getFd();
					/* Check if an event has occured on this fd */
					if (FD_ISSET(sd, &readfds)) {
						/* recv is the same as read */
						if ((valread = recv(sd, buffer, 1024, 0)) == 0)
						{  
							getpeername(sd , (struct sockaddr*)&address , \
								(socklen_t*)&addrlen);  
							printf("\e[0;34m[-] Host disconnected , ip %s , port %d \n\e[0m" , 
								inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
							
							close(sd); 
							/* Function to erase the client within the private attributs user */
							deleteClient(sd);
						}
			
						else {
							buffer[valread] = '\0';
							std::string tmp(buffer);
							std::string display = "What the server received : " + tmp;
							colorMsg("\e[0;96m", (char *)display.c_str());
							if (user[i]->histo.empty())
								user[i]->histo = tmp;
							if (user[i]->histo.find("\r\n") == std::string::npos) {
								user[i]->histo += tmp;
							}
							else {
								parse Parser((char *)user[i]->histo.c_str());
								Parser.to_register(*this, user[i]);
								user[i]->histo.clear();
							}
						}
					}
				}  	
			}
			return ;
		}

		void displayNbOfClient(void) {
			int connected = 0;
			for (size_t i = 0; i < user.size(); i++) {
				if (user[i]->accepted == true)
					connected++;
			}
			std::cout << "There is a total of : " << connected << " client connected on the server" << std::endl;
		}

		Client * findClientByFd(unsigned long s) {
			for (size_t i = 0; i < user.size(); i++) {
				if (user[i]->getFd() == s)
					return (user[i]);
			}
			return (NULL);
		}

		bool	clientExist(std::string name) {
			for (size_t i = 0; i < user.size(); i++) {
				if (name == user[i]->getNick())
					return true;
			}
			return false;
		}

		Client * findClientByName(std::string _name) {
			for (size_t i = 0; i < user.size(); i++) {
				if (_name == user[i]->getNick())
					return (user[i]);
			}
			return (NULL);
		}

		/* Channel Section */

		Channel*	findChannelByName(std::string _name) {
			for (size_t i = 0; i < Chan.size(); i++) {
				if (Chan[i]->getName() == _name)
					return (Chan[i]);
			}
			return (NULL);
		}

		/* This function serve to erase a client from the list of clients that a channel have.
		   If the client is the operators of the channel, it erases the channel form the list of
		   channel that the server have */
		void	delClientFromChann(Client * client, Channel * channel, Server & serv) {
			std::vector<Client *>::iterator	it = channel->members.begin();
			for (size_t i = 0; i < channel->members.size(); i++, it++) {
				if (channel->members[i]->getNick() == client->getNick()) {
					channel->members.erase(it);
					return ;
				}
				else if (channel->superUser[i]->getNick() == client->getNick()) {
					std::vector<Channel *>::iterator	ite = serv.Chan.begin();
					for (size_t i = 0; i < serv.Chan.size(); i++, ite++) {
						if (channel->getName() == serv.Chan[i]->getName()) {
							eraseAllClient(channel);
							serv.Chan.erase(ite);
							return ;
						}
					}
				}
			}
		}

		void	eraseAllClient(Channel * channel) {
			for (size_t i = 0; i < channel->members.size(); i++) {
				if (channel->members[i]->getCurr() == channel)
					channel->members[i]->setCurrChannelNull();
			}
		}
};

