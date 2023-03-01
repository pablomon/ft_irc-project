#ifndef OPER_CMD_HPP
#define OPER_CMD_HPP

#include "command.hpp"

class operCmd : public command
{
    public:
        operCmd(model *model, request request);
        ~operCmd();
        std::map<int, std::string> execute();
};

#endif