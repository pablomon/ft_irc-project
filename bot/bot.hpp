#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include <map>
#include <fcntl.h>

#define C_RESET "\033[0;0m"
#define EOMSG "\r\n"

struct CmdBot
{
	std::string					user;
	std::string					type;
	std::string					target;
	std::vector<std::string>	args;
};

class bot
{
	public:
		bot();
		bot(int port, std::string serverAddr, std::string pass);
		~bot();

		int connectBot();

		virtual void execute();
		CmdBot splitter(std::string msg);
		void rBeacon(std::string message);

		virtual void botController(CmdBot cmd) = 0;

		virtual bool help(CmdBot cmd) = 0;
		void warningMsg(CmdBot cmd);

		bool joinChannel(CmdBot cmd);
		bool leaveChannel(CmdBot cmd);
		bool changeNick(CmdBot cmd);
		bool setNick();
		bool setColor(CmdBot cmd);

		void close();
		void sendMsg(std::string target, std::string msg);

	protected:
		int _port;
		std::string _serverAddr;
		std::string _pass;

		std::string _nick;
		char _trigger;
		std::string _color;

		int _eop;
		int _socket;
		sockaddr_in _server;
};

#endif