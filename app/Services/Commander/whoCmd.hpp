#if !defined(WHO_CMD_HPP)
#define WHO_CMD_HPP

#include "command.hpp"

class whoCmd : public command
{
public:
    whoCmd(model *ptr_model, request req);
    ~whoCmd();
    std::map<int, std::string> execute();

    private:
    void doChannelWho();
    void doUserWho();
};

#endif
