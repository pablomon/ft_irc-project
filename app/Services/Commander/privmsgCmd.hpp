#ifndef PRIVMSG_CMD_HPP
#define PRIVMSG_CMD_HPP

#include "command.hpp"

class privmsgCmd : public command
{
public:
    privmsgCmd(model *ptr_model, request req);
    ~privmsgCmd();
    std::map<int, std::string> execute();
};

#endif