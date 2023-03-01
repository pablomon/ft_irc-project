#ifndef TOPICCMD_HPP
#define TOPICCMD_HPP

#include "command.hpp"

class topicCmd : public command
{
public:
    topicCmd(model *ptr_model, request req);
    ~topicCmd();
    std::map<int, std::string> execute();
};

#endif // TOPICCMD_HPP