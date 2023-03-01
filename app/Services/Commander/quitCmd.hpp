#if !defined(QUIT_CMD_HPP)
#define QUIT_CMD_HPP

#include "command.hpp"

class quitCmd : public command
{
public:
    quitCmd(model *ptr_model, request req);
    ~quitCmd();
    std::map<int, std::string> execute();
};

#endif // QUIT_CMD_HPP
