#pragma once 

#include "irc.hpp"


class SocketCreaFailed : public std::exception {
	public:
		virtual const char* what() const throw()	
		{
			return ("Socket Creation failed");
		}
};

class SetSocketOptFailed : public std::exception {
	public:
		virtual const char* what() const throw()	
		{
			return ("Set socket opt failed");
		}
};

class SocketBindFailed : public std::exception {
	public:
		virtual const char* what() const throw()	
		{
			return ("Socket bind failed");
		}
};

class SocketListenFailed : public std::exception {
	public:
		virtual const char* what() const throw()	
		{
			return ("Socket listen failed");
		}
};

class ServerAcceptFailed : public std::exception {
	public:
		virtual const char* what() const throw()	
		{
			return ("server accept failed...");
		}
};

class SelectFailed : public std::exception {
	public:
		virtual const char* what() const throw()	
		{
			return ("Select failed");
		}
};

class AcceptFailed : public std::exception {
	public:
		virtual const char* what() const throw()	
		{
			return ("Accept failed");
		}
};