#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "../../Model/model.hpp"
#include "../../Templates/request.hpp"
#include "../../Model/user.hpp"
#include "../../Templates/msgTools.hpp"

#include <vector>
#include <map>
#include <string>

// base class for commands
class command
{
public:
    command(model *ptr_model, request req);
    virtual ~command();
    virtual std::map<int, std::string> execute() = 0;

protected:
    model *ptr_model;
    request req;
    std::map<int, std::string> response;
};

#endif // COMMAND_HPP