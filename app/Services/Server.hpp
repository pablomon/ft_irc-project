#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <unistd.h>			/* close() C Library */

#include <fcntl.h>			/* Nonblocking socket */
#include <sys/poll.h>
#include <sys/socket.h>

#include <arpa/inet.h>		/* IP convertions */

#include "Socket.hpp"
#include "../Controller/ServicesController.hpp"

#include <map>
#include <set>

//  family TCP/IP
//AF_INET;
//  TCP
//SOCK_STREAM;
//  exit function
//EXIT_FAILURE;

class ServicesController;

class Server: public Socket
{
	private:
		struct var_irc
		{
			int			port;
			char		addr[255];
			sockaddr_in	serv_addr;
			int			listenSocketFd;
			struct pollfd fds[100];
			std::string buffers[100];
			int			nfds;
			int 		current_size;
			int 		max_listen_requests;
			int			compress_array;
			int 		buflen;
			int			end_server;
			int			close_con;
		};

		var_irc		_g_irc;
		std::string		_addr;
		int				_BUFLEN;
		std::string		_pass;
		std::string		_date;
		//
		ServicesController _SController;

		std::set<int> marked4kill;
	
	public:
		Server(char const *addr,  char const *port, std::string pass);
		~Server( void );
		//
		void	setEndServer(int value);
		int		run();
		void	scan_incoming_requests( void );
		void	receive_message(int i);
		void	create_new_fd( void );
		//
		void	BindPort( sockaddr_in sockaddr );
		void	ListenSocket();
		//
		void	mark4kill(int fd);
		class ErrorBindSocketErrno: public std::exception
		{
			public:
				virtual const char *what() const throw ();
		};
		class ErrorListenSocketErrno: public std::exception
		{
			public:
				virtual const char *what() const throw ();
		};

		std::string getPass();
		std::string getCreationDate();

};

#endif
