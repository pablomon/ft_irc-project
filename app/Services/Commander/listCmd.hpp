#if !defined(LIST_CMD_HPP)
#define LIST_CMD_HPP

#include "command.hpp"
#include <sstream>
#include <cstring>

class listCmd : public command
{
    public:
        listCmd(model *ptr_model, request req);
        ~listCmd();
        std::string listDisplay(channel *ch, int nUsers);
        std::map<int, std::string> execute();
};

#endif // LIST_CMD_HPP