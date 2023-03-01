#include "pingCmd.hpp"

pingCmd::pingCmd(model *ptr_model, request req) : command(ptr_model, req) {}
pingCmd::~pingCmd() {}

std::map<int, std::string> pingCmd::execute()
{
    // args num check
    if (req.args.size() == 0)
    {
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("461", "Need more parameters")));
        return response;
    }

    command::execute();
    std::string pong = composeMsg("PONG", req.args[0]);
    response.insert(std::pair<int, std::string>(req.senderfd, pong));
    return response;
}