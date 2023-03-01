#ifndef NOTICE_CMD_HPP
#define NOTICE_CMD_HPP

#include "command.hpp"

class noticeCmd : public command
{
public:
    noticeCmd(model *ptr_model, request req);
    ~noticeCmd();
    std::map<int, std::string> execute();
};

#endif