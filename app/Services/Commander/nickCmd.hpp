#if !defined(NICK_CMD_HPP)
#define NICK_CMD_HPP

#include "command.hpp"

class nickCmd : public command
{
public:
    nickCmd(model *ptr_model, request req, std::string creationDate);
    ~nickCmd();
    std::map<int, std::string> execute();

private:
    std::string _creationDate;    
};

#endif // NICK_CMD_HPP
