#ifndef KICK_CMD_HPP
#define KICK_CMD_HPP

#include "command.hpp"

class kickCmd : public command
{
    public:
		kickCmd(model *ptr_model, request req);
		~kickCmd();
		std::map<int, std::string> execute();
};

#endif // KICK_CMD_HPP
