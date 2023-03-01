#ifndef USER_CMD_HPP
#define USER_CMD_HPP

#include "command.hpp"

class userCmd : public command
{
public:
    userCmd(model *ptr_model, request req, std::string creationDate);
    ~userCmd();
    std::map<int, std::string> execute();

private:
    std::string _creationDate;
};

#endif // !USER_CMD_HPP