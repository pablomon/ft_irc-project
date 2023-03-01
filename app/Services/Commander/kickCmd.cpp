#include "kickCmd.hpp"

kickCmd::kickCmd(model *ptr_model, request req) : command(ptr_model, req)
{
}

kickCmd::~kickCmd() {}

std::map<int, std::string> kickCmd::execute()
{
    // args num check
    if (req.args.size() == 0)
    {
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("461", "Need more parameters")));
        return response;
    }

    std::string reason = "";
    if (req.args.size() > 1)
        reason = req.args[2];

    std::string user_nick = req.args[1];
    std::cout << user_nick << std::endl;
    user *target = ptr_model->getUserByNick(user_nick.c_str());
    if (target)
    {
        int fd_target = target->getFd();

        channel *chan = ptr_model->getChannel(req.args[0]);
        if (chan == NULL)
        {
            response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("403", "No such channel: " + req.args[0])));
            return response;
        }
        // check if user is in channel
        if (!ptr_model->isUserInChannel(fd_target, chan))
        {
            response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("441", "User not on that channel: " + req.args[0])));
            return response;
        }
        if (!ptr_model->isUserInChannel(req.senderfd, chan))
        {
            response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("442", "You're not on that channel: " + req.args[0])));
            return response;
        }
        //

        user *sender = ptr_model->getUser(req.senderfd);
        channel_user *channel_user = ptr_model->getChannelUser(req.senderfd, chan);

        bool isOp = false;
        if (channel_user->isChanop)
            isOp = true;
        if (sender->getIsOperator())
            isOp = true;

        if (!isOp)
        {
            response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("482", "No operator in: " + req.args[0])));
            return response;
        }

        std::string kick = composeMsg("KICK", chan->getName(), user_nick);
        kick = addPrefix(sender->getPrefix(), kick);
        //
        std::vector<user *> channelUsers = ptr_model->getUsersInChannel(chan);
        for (size_t j = 0; j < channelUsers.size(); j++)
            response.insert(std::pair<int, std::string>(channelUsers[j]->getFd(), kick));

        ptr_model->removeUserFromChannel(target->getFd(), chan);
    }
    else
    {
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("401", "No User nick: " + req.args[0])));
        return response;
    }
    return response;
}
