#pragma once

#include "irc.hpp"

class Channel;

class Client {
	private:
			/* Client Info */
		std::string		_password;
		std::string		_nickname;
		std::string 	_realname;
		std::string 	_username;
		
		// char			mode;
		Channel*		_curr;
		/* The fd where the client send his socket and where I received it */
		unsigned long	fd;

	public:
		/* This bools consists that it turn true when the client send one of these info
			so I know that this info have been set so when all of these turn true, 
			I accept the client in the server */
		bool	pass;
		bool	nick;
		bool	user;

		bool	accepted; // Check if the user is accepted by the server

		/* This bool serves to know if the client is op on the Server,
			not on a Channel, when he's op he can execute operator commands like Kill */
		bool	op;
		
		/* What the client send is stocked in this string, if there isn't "\r\n", 
			the server doesn't parse what the client has send */
		std::string		histo;

		bool	real;

	public:
		Client() : _password() { 
			fd = 0;
			pass = false;
			nick = false;
			real = false;
			user = false;
			accepted = false;
			op = false;

			_curr = NULL;
		}

		Client(unsigned long _n) {
			fd = _n;
			pass = false;
			nick = false;
			real = false;
			user = false;
			accepted = false;
		}

		Client(const Client & rhs) {
			*this = rhs;
		}

		~Client() { }

		Client & operator=(const Client & rhs) {
			_nickname = rhs._nickname;
			_realname = rhs._realname;
			_username = rhs._username;
			// _curr = rhs._curr;
			fd = rhs.fd;
			return (*this);
		}

		void	setFd(int _n) {
			fd = _n;
		}

		unsigned long getFd(void) {
			return (fd);
		}

		std::string getPwd(void) {
			return (_password);
		}

		void	setPwd(std::string name) {
			_password = name;
		}

		std::string getNick(void) {
			return (_nickname);
		}

		void	setNick(std::string nick) {
			_nickname = nick;
		}

		std::string getUser(void) {
			return (_username);
		}
		
		void	setUser(std::string user) {
			_username = user;
		}

		void	setReal(std::string real) {
			_realname = real;
		}

		Channel*	getCurr(void) {
			return (_curr);
		}
		
		void	setCurrChannel(Channel * set) {
			_curr = set;
		}

		void	setCurrChannelNull(void) {
			_curr = NULL;
		}
};