#include "privmsgCmd.hpp"

privmsgCmd::privmsgCmd(model *ptr_model, request req) : command(ptr_model, req) {}
privmsgCmd::~privmsgCmd() {}

std::map<int, std::string> privmsgCmd::execute()
{
    // args num check
    if (req.args.size() == 0)
    {
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("461", "Need more parameters")));
        return response;
    }

    command::execute();

    user *senderUser = ptr_model->getUser(req.senderfd);
    std::string senderNick = senderUser->getNick();

    std::string target = req.args[0]; // note that irssi takes care of comma separated targets

    // send to channel
    if (target[0] == '#')
    {
        std::cout << "target = " << target << std::endl;
        // check if the channel exists
        channel *channel = ptr_model->getChannel(target);
        if (channel == NULL)
        {
            std::string msg = composeMsg("403", senderNick, target, "No such a channel");
            response.insert(std::pair<int, std::string>(req.senderfd, msg));
            return response;
        }
        // check if the user is in the channel
        if (!ptr_model->isUserInChannel(req.senderfd, channel))
        {
            std::string msg = composeMsg("404", senderNick, target, "You are not in this channel");
            response.insert(std::pair<int, std::string>(req.senderfd, msg));
            return response;
        }

        // send the message to all users in the channel except the sender
        std::vector<user *> users = ptr_model->getUsersInChannel(channel);
        std::string msg = composeMsg("PRIVMSG", target, req.args[1]);
        msg = addPrefix(senderUser->getPrefix(), msg);
        for (size_t i = 0; i < users.size(); i++)
        {
            if (users[i]->getFd() != req.senderfd)
                response.insert(std::pair<int, std::string>(users[i]->getFd(), msg));
        }
        return response;
    }

    // send to user
    // Check if the recipient exists
    user *recipient = ptr_model->getUserByNick(req.args[0].c_str());
    if (recipient == NULL)
    {
        std::string msg = composeMsg("401", senderNick, req.args[0], "No such a nick");
        response.insert(std::pair<int, std::string>(req.senderfd, msg));
        return response;
    }

    // Send the message
    std::string msg = composeMsg("PRIVMSG", req.args[0], req.args[1]);
    msg = addPrefix(senderUser->getPrefix(), msg);
    response.insert(std::pair<int, std::string>(recipient->getFd(), msg));
    return response;
}