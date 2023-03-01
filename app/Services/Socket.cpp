#include "Socket.hpp"

Socket::Socket(void)
{}
Socket::~Socket(void)
{}

int Socket::getListenSocketFd( void )
{    
	try
    {
        if((this->_listenSocketFd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            throw ErrorCreateSocketException();
        }
        std::cout << "Create Socket" << std::endl;
        return (this->_listenSocketFd); 
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << errno << '\n';
        exit(EXIT_FAILURE);
		return (-1);
    }
    
}

// Set socket to be nonblocking
void Socket::getFlagFcntl( void )
{
	try
    {
        this->_flags = fcntl(this->_listenSocketFd , F_GETFL);
        if (fcntl(this->_listenSocketFd, F_SETFL, this->_flags | O_NONBLOCK) < 0)
        {
            throw ErrorFailedFctlErrno();
        }
        std::cout << "Create Flag fctl" << std::endl;
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << errno << '\n';
        close(this->_listenSocketFd);
        exit(EXIT_FAILURE);
    }//
}

// Errors

const char *Socket::ErrorCreateSocketException::what(void) const throw( )
{
    return "Failed to create socket. errno:";
}

const char *Socket::ErrorFailedFctlErrno::what(void) const throw( )
{
    return "Failed fctl. errno:";
}

const char *Socket::ErrorBindSocketErrno::what(void) const throw( )
{
    return "Failed to bind to port. errno:";
}
