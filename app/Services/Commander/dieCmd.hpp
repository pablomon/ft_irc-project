#if !defined(DIE_CMD_HPP)
#define DIE_CMD_HPP

#include "command.hpp"
#include "../Server.hpp"

class dieCmd : public command
{
    public:
        dieCmd(model *model, request reques, Server *ptr_server);
        ~dieCmd();
        std::map<int, std::string> execute();

    private:
        Server *_ptr_server;
};

#endif // die_CMD_HPP