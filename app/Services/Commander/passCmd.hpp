#ifndef PASS_CMD_HPP
#define PASS_CMD_HPP

#include "command.hpp"
#include "../Server.hpp"


class passCmd : public command
{
    public:
		passCmd(model *ptr_model, request req, Server *ptr_server);
		~passCmd();
		std::map<int, std::string> execute();

	private:
		Server *_ptr_server;
};

#endif // PASS_CMD_HPP
