#include "noticeCmd.hpp"

noticeCmd::noticeCmd(model *ptr_model, request req) : command(ptr_model, req) {}
noticeCmd::~noticeCmd() {}

std::map<int, std::string> noticeCmd::execute()
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

    std::string target = req.args[0]; // note that irssi takes care of comma separated targets before sending the request to the server

    // send to channel
    if (target[0] == '#')
    {
        // check if the channel exists
        channel *channel = ptr_model->getChannel(target);
        if (channel == NULL)
            return response;

        // check if the user is in the channel
        if (!ptr_model->isUserInChannel(req.senderfd, channel))
            return response;

        // send the message to all users in the channel except the sender ( NOTICE never sends a reply )
        std::vector<user *> users = ptr_model->getUsersInChannel(channel);
        std::string msg = composeMsg("NOTICE", target, req.args[1]);
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
        return response;

    // Send the message
    std::string msg = composeMsg("NOTICE", req.args[0], req.args[1]);
    msg = addPrefix(senderUser->getPrefix(), msg);
    response.insert(std::pair<int, std::string>(recipient->getFd(), msg));
    return response;
}