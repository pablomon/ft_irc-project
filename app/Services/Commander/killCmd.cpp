#include "killCmd.hpp"

killCmd::killCmd(model *model, request request, Server *ptr_server) : command(model, request)
{
    _ptr_server = ptr_server;
}

killCmd::~killCmd()
{
}

std::map<int, std::string> killCmd::execute()
{
    // args num check
    if (req.args.size() == 0)
    {
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("461", "Need more parameters")));
        return response;
    }

    std::string msg;
    // check if user is ircop
    user *op = ptr_model->getUser(req.senderfd);
    if (!op->getIsOperator())
    {
        msg = composeMsg("481", op->getNick(), "Permission Denied- You're not an IRC operator");
        response.insert(std::pair<int, std::string>(req.senderfd, msg));
        return response;
    }

    // check if the user exists
    user *target = ptr_model->getUserByNick(req.args[0].c_str());
    if (target == NULL)
    {
        msg = composeMsg("401", op->getNick(), req.args[0], "No such a nick");
        response.insert(std::pair<int, std::string>(req.senderfd, msg));
        return response;
    }

    // skip if sender and target are the same
    if (op->getFd() == target->getFd())
        return response;

    // mark user for kill
    _ptr_server->mark4kill(target->getFd());
    // add kill message to target
    msg = composeMsg("KILL", req.args[0], req.args[1]);
    msg = addPrefix(op->getPrefix(), msg);

    // send quit message to user being kill
    std::string quitMsg = composeMsg("QUIT", target->getNick(), "Killed");
    quitMsg = addPrefix(target->getPrefix(), quitMsg);
    msg.append(quitMsg);
    // send error message to target
    std::string errorMsg = composeMsg("ERROR", "Closing Link: " + target->getNick() + " (Killed)");
    msg.append(errorMsg);
    response.insert(std::pair<int, std::string>(target->getFd(), msg));

    // send quit message to the channel
    std::vector<channel *> channels = ptr_model->getChannelsByUser(target->getFd());
    for (std::vector<channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        std::vector<user *> channelUsers = ptr_model->getUsersInChannel(*it);
        for (std::vector<user *>::iterator it2 = channelUsers.begin(); it2 != channelUsers.end(); ++it2)
            response.insert(std::pair<int, std::string>((*it2)->getFd(), quitMsg));
    }

    return response;
}