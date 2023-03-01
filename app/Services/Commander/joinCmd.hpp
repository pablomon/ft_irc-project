#if !defined(JOIN_CMD_HPP)
#define JOIN_CMD_HPP

#include "command.hpp"

class joinCmd : public command
{
    public:
    joinCmd(model *ptr_model, request req);
    ~joinCmd();
    std::map<int, std::string> execute();
};

#endif // JOIN_CMD_HPP
