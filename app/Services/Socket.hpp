/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan-gon <juan-gon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 08:32:53 by juan-gon          #+#    #+#             */
/*   Updated: 2022/12/19 22:31:47 by juan-gon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <stdlib.h>
#include <unistd.h> // For file descriptors
#include <fcntl.h> // for non blocking socket
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h> // for ip convertions sockaddr_in

/***
 * Creation socket listen
 ***/

class Socket
{
	protected:
		int			_AF_INET;
		int			_SOCK_STREAM;
	private:
		int			_listenSocketFd;
		int			_flags;
	public:
		Socket( void );
		~Socket( void );
		//
        //
		int		getListenSocketFd( void );
		void	getFlagFcntl( void );
		void	getFlagFcntl(sockaddr_in sockaddr, char *server_addr, int server_port);
		//
		//
		class ErrorCreateSocketException: public std::exception
		{
			public:
				virtual const char *what() const throw ();
		};
		class ErrorFailedFctlErrno: public std::exception
		{
			public:
				virtual const char *what() const throw ();
		};
		class ErrorBindSocketErrno: public std::exception
		{
			public:
				virtual const char *what() const throw ();
		};
};
#endif
