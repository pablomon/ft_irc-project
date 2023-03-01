#include "operCmd.hpp"

operCmd::operCmd(model *ptr_model, request req) : command(ptr_model, req) {}
operCmd::~operCmd() {}

std::map<int, std::string> operCmd::execute()
{
    // args num check
    if (req.args.size() == 0)
    {
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("461", "Need more parameters")));
        return response;
    }

    std::cout << "operCmd::execute()" << std::endl;
    // check if user is already an operator
    user *u = ptr_model->getUser(req.senderfd);

    if (u->getIsOperator())
        return response;

    // check if nick and pass are correct
    std::string nick = u->getNick();
    std::string pass = req.args[0];
    if (req.args.size() == 2)
    {
        nick = req.args[0];
        pass = req.args[1];
    }

    if (nick == "admin" && pass == "admin")
    {
        u->setIsOperator(true);
        std::string okmsg = composeMsg("381", "You are now an IRC operator");
        okmsg = addPrefix(u->getPrefix(), okmsg);
        std::string modemsg = composeMsg("MODE", u->getNick(), "+o");
        modemsg = addPrefix(u->getPrefix(), modemsg);
        std::string msg = okmsg;
        msg.append(modemsg);
        response.insert(std::pair<int, std::string>(req.senderfd, msg));
    }
    else
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("464", "Password missmatch")));

    return response;
}