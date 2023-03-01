#if !defined(KILL_CMD_HPP)
#define KILL_CMD_HPP

#include "command.hpp"
#include "../Server.hpp"

class killCmd : public command
{
    public:
        killCmd(model *model, request reques, Server *ptr_server);
        ~killCmd();
        std::map<int, std::string> execute();

    private:
        Server *_ptr_server;
};

#endif // KILL_CMD_HPP