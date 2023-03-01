#ifndef MODE_CMD_HPP
#define MODE_CMD_HPP

#include "command.hpp"

class modeCmd : public command
{
public:
    modeCmd(model *ptr_model, request req);
    ~modeCmd();
    std::map<int, std::string> execute();
    void doUserMode();
    void doChannelMode();
};

#endif