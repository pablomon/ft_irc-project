#include "dieCmd.hpp"

dieCmd::dieCmd(model *model, request request, Server *ptr_server) : command(model, request)
{
    _ptr_server = ptr_server;
}

dieCmd::~dieCmd()
{
}

std::map<int, std::string> dieCmd::execute()
{
    std::string msg;
    // check if user is ircop
    user *op = ptr_model->getUser(req.senderfd);
    if (!op->getIsOperator())
    {
        msg = composeMsg("481", op->getNick(), "Permission Denied- You're not an IRC operator");
        response.insert(std::pair<int, std::string>(req.senderfd, msg));
        return response;
    }

    _ptr_server->setEndServer(1);

    return response;
}