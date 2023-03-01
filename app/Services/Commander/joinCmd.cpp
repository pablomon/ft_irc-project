

#include "joinCmd.hpp"

joinCmd::joinCmd(model *ptr_model, request req) : command(ptr_model, req) {}
joinCmd::~joinCmd() {}
std::map<int, std::string> joinCmd::execute()
{
    command::execute();
    channel *channel = NULL;

    // args num check
    if (req.args.size() == 0)
    {
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("461", "Need more parameters")));
        return response;
    }

    // parse args
    std::vector<std::string> arg0split = split(req.args[0], ",");
    std::vector<std::string> arg1split;
    if (req.args.size() > 1)
    {
        std::vector<std::string> tmp = split(req.args[1], ",");
        arg1split.insert(arg1split.end(), tmp.begin(), tmp.end());
    }

    std::map<std::string, std::string> channels2join;

    for (size_t i = 0; i < arg0split.size(); i++)
    {
        // find corresponding key
        std::string channelName = arg0split[i];
        if (arg1split.size() > i)
            channels2join.insert(std::pair<std::string, std::string>(channelName, arg1split[i]));
        else
            channels2join.insert(std::pair<std::string, std::string>(channelName, ""));

        // si el canal no existe, crearlo
        if (channelName.front() != '#')
        {
            user *u = ptr_model->getUser(req.senderfd);
            std::string joinMsg = composeMsg("403", "No such channel", channelName);
            joinMsg = addPrefix(u->getPrefix(), joinMsg);
            response.insert(std::pair<int, std::string>(req.senderfd, joinMsg));
            return (response);
        }

        channel = ptr_model->getChannel(channelName);
        if (channel == NULL)
        {
            channel = ptr_model->newChannel(channelName);
            user *u = ptr_model->getUser(req.senderfd);
            ptr_model->addUserToChannel(u->getFd(), channel)->isChanop = true;
            std::string joinMsg = composeMsg("JOIN", channel->getName());
            joinMsg = addPrefix(u->getPrefix(), joinMsg);
            response.insert(std::pair<int, std::string>(req.senderfd, joinMsg));
            continue;
        }

        // si el canal existe, verificar que el usuario no este en el
        user *u = ptr_model->getUser(req.senderfd);
        if (ptr_model->isUserInChannel(u->getFd(), channel))
        {
            std::string errMsg = composeMsg("ERROR", "You are already in this channel");
            response.insert(std::pair<int, std::string>(req.senderfd, errMsg));
            continue;
        }

        // Add user to channel and send channel info:
        ptr_model->addUserToChannel(u->getFd(), channel);
        std::string joinMsg = composeMsg("JOIN", channel->getName());
        joinMsg = addPrefix(u->getPrefix(), joinMsg);

        // 0. join message
        std::string infoMsg = joinMsg;
        // 1. topic
        infoMsg.append(composeMsg("332", u->getNick(), channel->getName(), channel->getTopic().topic));
        // 2. topic setter and date
        infoMsg.append(composeMsg("333", u->getNick(), channel->getName(), channel->getTopic().setter, channel->getTopic().date));
        // 3. names
        std::vector<user *> channelUsers = ptr_model->getUsersInChannel(channel);
        std::string nicks = ptr_model->getChannelNicks(channel);
        infoMsg.append(composeMsg("353", u->getNick(), "=", channel->getName(), nicks));
        // 4. end of names
        infoMsg.append(composeMsg("366", u->getNick(), channel->getName(), "End of /NAMES list."));
        response.insert(std::pair<int, std::string>(req.senderfd, infoMsg));

        // avisar a los usuarios del canal que se ha unido un nuevo usuario
        for (size_t i = 0; i < channelUsers.size(); i++)
            response.insert(std::pair<int, std::string>(channelUsers[i]->getFd(), joinMsg));
    }
    return response;
}