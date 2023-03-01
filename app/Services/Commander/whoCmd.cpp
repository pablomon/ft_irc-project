#include "whoCmd.hpp"

whoCmd::whoCmd(model *ptr_model, request req) : command(ptr_model, req)
{
}

whoCmd::~whoCmd()
{
}

std::map<int, std::string> whoCmd::execute()
{
    // Comprueba que el número de parametros sea correcto
    if (req.args.size() == 0)
    {
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("461", "Not enough parameters")));
        return response;
    }

    // check whether is a channel
    if (req.args[0][0] == '#')
        doChannelWho();
    else
        doUserWho();

    return response;
}

std::string getUserInfo(model *ptr_model, user *u, channel *chan)
{
    std::string flags = "";
    if (chan != NULL)
        flags = ptr_model->getChannelUser(u->getFd(), chan)->userFlags(u);
    else
        flags = "H"; // user is always here

    if (u->getIsOperator())
        flags += "O";
    std::string userInfo = u->getUsername() + " " + u->getHostname() + " " + u->getIp() + " " + u->getNick() + " " + flags;
    return userInfo;
}

void whoCmd::doUserWho()
{
    user *u = ptr_model->getUser(req.senderfd);
    user *targetUser = ptr_model->getUserByNick(req.args[0].c_str());
    std::string msg = "";
    if (targetUser != NULL)
    {
        std::string userInfo = getUserInfo(ptr_model, targetUser, NULL);
        std::string trail = "0 " + targetUser->getRealname(); // hopcount always 0
        msg.append(composeMsg("352", targetUser->getNick(), "*", userInfo, trail));
    }

    msg.append(composeMsg("315", u->getNick(), "End of /WHO list."));
    response.insert(std::pair<int, std::string>(req.senderfd, msg));
}

void whoCmd::doChannelWho()
{
    std::string msg = "";
    user *u = ptr_model->getUser(req.senderfd);
    channel *chan = ptr_model->getChannel(req.args[0]);
    if (chan == NULL)
    {
        msg.append(composeMsg("315", u->getNick(), req.args[0], "End of /WHO list."));
        return;
    }

    // check if user is in channel
    if (!ptr_model->isUserInChannel(req.senderfd, chan))
    {
        msg.append(composeMsg("315", u->getNick(), chan->getName(), "End of /WHO list."));
        return;
    }

    // get users in channel
    std::vector<user *> users = ptr_model->getUsersInChannel(chan);
    for (std::vector<user *>::iterator it = users.begin(); it != users.end(); ++it)
    {
        std::string userInfo = getUserInfo(ptr_model, (*it), chan);
        std::string trail = "0 " + (*it)->getRealname(); // hopcount always 0
        msg.append(composeMsg("352", u->getNick(), chan->getName(), userInfo, trail));
    }

    // end of names
    msg.append(composeMsg("315", u->getNick(), chan->getName(), "End of /WHO list."));
    response.insert(std::pair<int, std::string>(req.senderfd, msg));
}