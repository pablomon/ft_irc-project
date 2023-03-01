#include "partCmd.hpp"

partCmd::partCmd(model *ptr_model, request req) : command(ptr_model, req) {}

partCmd::~partCmd() {}

std::map<int, std::string> partCmd::execute()
{
    // args num check
    if (req.args.size() == 0)
    {
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("461", "Need more parameters")));
        return response;
    }

    std::string reason = "";
    if (req.args.size() > 1)
        reason = req.args[1];
    std::vector<std::string> channels = split(req.args[0], ",");

    for (size_t i = 0; i < channels.size(); i++)
    {
        channel *chan = ptr_model->getChannel(channels[i]);
        // check if channel exists
        if (chan == NULL)
        {
            response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("403", "No such channel: " + channels[i])));
            continue;
        }

        // check if user is in channel
        if (!ptr_model->isUserInChannel(req.senderfd, chan))
        {
            response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("442", "You're not on that channel: " + chan->getName())));
            continue;
        }

        // compose message to send to all users in channel
        user *sender = ptr_model->getUser(req.senderfd);
        if (reason == "")
            reason = sender->getNick() + " has left the channel";

        std::string partMsg = composeMsg("PART", chan->getName(), chan->getNameNoPrexi());
        partMsg = addPrefix(sender->getPrefix(), partMsg);

        // get all users in channel
        std::vector<user *> channelUsers = ptr_model->getUsersInChannel(chan);
        for (size_t j = 0; j < channelUsers.size(); j++)
            response.insert(std::pair<int, std::string>(channelUsers[j]->getFd(), partMsg));

        // remove user from channel
        ptr_model->removeUserFromChannel(req.senderfd, chan);
    }

    return response;
}