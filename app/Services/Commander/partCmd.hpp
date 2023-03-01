#if !defined(PART_CMD_HPP)
#define PART_CMD_HPP

#include "command.hpp"

class partCmd : public command
{
public:
    partCmd(model *ptr_model, request req);
    ~partCmd();
    std::map<int, std::string> execute();
};

#endif // PART_CMD_HPP
