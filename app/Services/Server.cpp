#include "Server.hpp"

#include <set>
#include <sstream>

Server::Server(char const *addr,  char const *port, std::string pass)
{
	_pass = pass;
	
	std::cout << "ctor char char" << std::endl;
	//
	strcpy(this->_g_irc.addr, addr);
	this->_g_irc.port					= std::atoi(port);
	//
	// bufflen repair posr
	this->_g_irc.buflen					= 1024;
	//
	this->_g_irc.listenSocketFd			= this->getListenSocketFd();
	//
	this->_g_irc.nfds					= 5;
	this->_g_irc.current_size			= 0;
	this->_g_irc.max_listen_requests	= 100;
	this->_g_irc.end_server				= 0;
	this->_g_irc.close_con				= 0;
	//
	this->_g_irc.serv_addr.sin_family		= AF_INET;
	this->_g_irc.serv_addr.sin_addr.s_addr	= INADDR_ANY;
	this->_g_irc.serv_addr.sin_port			= htons(this->_g_irc.port);
	//
	this->getFlagFcntl();
	this->BindPort(this->_g_irc.serv_addr);
	this->ListenSocket();
	//
	// Initialize the pollfd structure
    memset(this->_g_irc.fds, 0, sizeof(this->_g_irc.fds));
    this->_g_irc.fds[0].fd = this->_g_irc.listenSocketFd;
    this->_g_irc.fds[0].events = POLLIN;
	//
	this->_BUFLEN = 100;
	// Set creation date
	std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
	std::stringstream dateStream;
	dateStream << (now->tm_year + 1900) << '-' 
         << (now->tm_mon + 1) << '-'
         <<  now->tm_mday << " "
		 << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec;
  	_date = dateStream.str();
	// Run server
	_SController.setServer(this);
    std::cout << "Server initialized on " << _date << std::endl;
	std::cout << "Server connection password: " << _pass << std::endl;
	std::cout << std::endl;
	this->run();
}

Server::~Server( void )
{}

std::string Server::getCreationDate() { return _date; }
std::string Server::getPass() { return _pass; }
void Server::setEndServer(int value) { this->_g_irc.end_server = value; }

int Server::run()
{
	while(!this->_g_irc.end_server)
	{
		int poll_count = poll(this->_g_irc.fds, this->_g_irc.nfds, -1);
		if (poll_count < 0)
		{
			std::cerr << "poll() failed: " << errno << std::endl;
			break;
		}
		if (poll_count == 0)
		{
			std::cerr << "poll() timed out.  End program." << std::endl;
			break;
		}

		this->_g_irc.current_size = this->_g_irc.nfds;
        this->_g_irc.compress_array = 0;
		this->scan_incoming_requests();
	}
	// Clean up all the sockets that are open
    for (int i = 0; i < this->_g_irc.nfds; i++)
    {
        if (this->_g_irc.fds[i].fd >= 0)
            close(this->_g_irc.fds[i].fd);
    }
    close(this->_g_irc.listenSocketFd);
    std::cout << "Server closed.\n" << std::endl;
	return 0;
}

void Server::scan_incoming_requests( void )
{
	for (int i = 0; i < this->_g_irc.current_size; i++)
	{
		if (this->_g_irc.fds[i].revents == 0)
		{
			continue;
		}

		if (this->_g_irc.fds[i].fd == this->_g_irc.listenSocketFd)
		{
			std::cout << "Listening socket is readable" << std::endl;
			this->create_new_fd();
		}
		else
		{
			this->receive_message(i);
		}
        if (this->_g_irc.compress_array)
        {
            this->_g_irc.compress_array = 0;
            for (int i = 0; i < this->_g_irc.nfds; i++)
            {
                if (this->_g_irc.fds[i].fd == -1)
                {
                    for (int j = i; j < this->_g_irc.nfds - 1; j++)
                        this->_g_irc.fds[j].fd =this->_g_irc.fds[j + 1].fd;
                    i--;
                    this->_g_irc.nfds--;
                }
            }
        }
	}
}
void Server::receive_message(int i)
{
	this->_g_irc.close_con = 0;                
	int rc = 1;

	std::string message;
	while (rc > 0) // rc > 0 means there were no errors and the connection remains open
	{
		char buffer[this->_BUFLEN];
		memset(&buffer, 0, this->_BUFLEN);

		rc = recv(this->_g_irc.fds[i].fd, buffer, sizeof(buffer), 0);
		if (rc < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				perror("recv() failed");
				this->_g_irc.close_con = 1;
				break;
			}
		}
		if (rc == 0) // Conexion closed by client
		{
			std::cout << "Connection closed\r\n" << std::endl;
			this->_g_irc.close_con = 1;
			break;
		}
		this->_g_irc.buffers[i].append(buffer);
	}

	if (message.find("PING") == std::string::npos)
	{
		std::cout << ">fd(" << _g_irc.fds[i].fd << ")->" << this->_g_irc.buffers[i] << std::endl;
	}
	
	if(this->_g_irc.buffers[i].find("\r\n") != std::string::npos)
	{
		std::map<int,std::string> response = _SController.getCommandResponse(_g_irc.fds[i].fd, this->_g_irc.buffers[i]);
		for (std::map<int, std::string>::iterator it = response.begin(); it != response.end(); ++it)
		{
			if (it->second.find("PONG") == std::string::npos)
				std::cout << "<fd(" << it->first << ")->" << it->second.c_str() << std::endl;
			send(it->first, it->second.c_str(), it->second.length(), 0);
		}
		this->_g_irc.buffers[i].clear();
	}

    for (std::set<int>::reverse_iterator it = marked4kill.rbegin(); it != marked4kill.rend(); ++it)
    {
        int fd2kill = *it;
        std::cout << "KILLING connection with fd " << fd2kill << std::endl;
        _SController.deregisterUser(fd2kill);
        close(fd2kill);
		std::cout << "connection to fd " << fd2kill << " closed" << std::endl;
		for (int x=0; x < _g_irc.nfds; x++)
			if (this->_g_irc.fds[x].fd == fd2kill)
        		this->_g_irc.fds[x].fd = -1;
        this->_g_irc.compress_array = 1;
    }
	marked4kill.clear();

	if (this->_g_irc.close_con)
	{
		std::cout << "closing connection " << this->_g_irc.fds[i].fd << std::endl;
		_SController.deregisterUser(this->_g_irc.fds[i].fd);
		close(this->_g_irc.fds[i].fd);
		std::cout << "close(" << i << ")" << std::endl;
		this->_g_irc.fds[i].fd = -1;
		this->_g_irc.compress_array = 1;
		this->_g_irc.close_con = 0;
	}
}

void Server::create_new_fd( void )
{
	struct sockaddr_storage client_addr;
	char str[INET_ADDRSTRLEN];
	struct sockaddr_in *ptr;
	socklen_t addr_len;

	addr_len = sizeof(struct sockaddr_storage);
	int new_fd = 0;
	while (new_fd != -1)
	{
		if ((new_fd = accept(this->_g_irc.listenSocketFd, (struct sockaddr *)&client_addr, &addr_len)) < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				perror("  accept() failed");
				this->_g_irc.end_server = 1;
			}
			break;
		}
		if (client_addr.ss_family == AF_INET)
		{
			ptr = (struct sockaddr_in *)&client_addr;
			inet_ntop(AF_INET, &(ptr->sin_addr), str, sizeof(str));
		}
		else
		{
			ptr = NULL;
			std::cout << "Address family is not AF_INET\n" << std::endl;
		}
		int flags = fcntl(new_fd, F_GETFL);
		if (fcntl(new_fd, F_SETFL, flags | O_NONBLOCK) < 0)
		{
			std::cout << "Failed fctl. errno: " << errno << std::endl;
			close(new_fd);
			exit(-1);
		}
		if (ptr)
		{
			std::cout << "Connection from client: " << str << std::endl;
			this->_g_irc.fds[this->_g_irc.nfds].fd = new_fd;
			this->_g_irc.fds[this->_g_irc.nfds].events = POLLIN;
			this->_g_irc.nfds++;
			_SController.registerUser(new_fd);
		}
	}
}

// Set socket to be nonblocking sockaddr_in
void Server::BindPort(sockaddr_in serv_addr)
{
    try
    {
        if (bind(this->_g_irc.listenSocketFd , (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            throw ErrorBindSocketErrno();
        }
        std::cout << "Binding socket to ip " << this->_g_irc.addr << ":" << this->_g_irc.port << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << errno << '\n';
        close(this->_g_irc.listenSocketFd);
        exit(EXIT_FAILURE);
    }
}

void Server::ListenSocket()
{
    try
    {
		if (listen(this->_g_irc.listenSocketFd, this->_g_irc.max_listen_requests) < 0)
		{
			throw ErrorListenSocketErrno();
		}
		std::cout << "Listening on socket.." << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << errno << '\n';
        close(this->_g_irc.listenSocketFd);
        exit(EXIT_FAILURE);
    }
}

void Server::mark4kill(int fd)
{
	marked4kill.insert(fd);
}

// ErrorBindSocketErrno
const char *Server::ErrorBindSocketErrno::what(void) const throw( )
{
    return "Failed to bind to port. errno:";
}

const char *Server::ErrorListenSocketErrno::what(void) const throw( )
{
    return "Failed to listen on socket. errno:";
}