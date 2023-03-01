#ifndef PING_CMD_HPP
#define PING_CMD_HPP

#include "command.hpp"

class pingCmd : public command
{
public:
    pingCmd(model *ptr_model, request req);
    ~pingCmd();
    std::map<int, std::string> execute();
};

#endif // PINGCMD_HPP