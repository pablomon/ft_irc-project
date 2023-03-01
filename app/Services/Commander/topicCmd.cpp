#include "topicCmd.hpp"
#include <ctime>
#include <sstream>

topicCmd::topicCmd(model *ptr_model, request req) : command(ptr_model, req) {}

topicCmd::~topicCmd() {}

std::map<int, std::string> topicCmd::execute()
{
    // args num check
    if (req.args.size() == 0)
    {
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("461", "Need more parameters")));
        return response;
    }

    std::string senderNick = ptr_model->getUser(req.senderfd)->getNick();
    // check if channel exists
    channel *chan = ptr_model->getChannel(req.args[0]);
    if (chan == NULL)
    {
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("403", senderNick, req.args[0], "No such channel")));
        return response;
    }

    // check if user is in channel
    if (!ptr_model->isUserInChannel(req.senderfd, chan))
    {
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("442", senderNick, chan->getName(), "You're not on that channel")));
        return response;
    }

    // if not topic is given, return current topic
    if (req.args.size() == 1)
    {
        if (!chan->getTopic().isSet)
        {
            response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("331", chan->getName(), "No topic is set")));
            return response;
        }

        std::string info = composeMsg("332", chan->getName(), chan->getTopic().topic);
        info.append(composeMsg("333", chan->getName(), chan->getTopic().setter, chan->getTopic().date));
        response.insert(std::pair<int, std::string>(req.senderfd, info));
        return response;
    }

    // check if user is operator
    if (!ptr_model->getChannelUser(req.senderfd, chan)->isChanop)
    {
        std::string msg = composeMsg("482", ptr_model->getUser(req.senderfd)->getNick(), chan->getName(), "You're not channel operator");
        msg = addPrefix(ptr_model->getUser(req.senderfd)->getPrefix(), msg);
        response.insert(std::pair<int, std::string>(req.senderfd, msg));
        return response;
    }

    // set topic
    struct topic &topic = chan->getTopic();
    topic.clear();
    topic.setter = ptr_model->getUser(req.senderfd)->getPrefix();
    // get current time
    time_t now = time(0);
    // convert now to string form
    std::ostringstream ss;
    ss << now;
    topic.date = ss.str();
    topic.topic = req.args[1];
    topic.isSet = true;

    // compose message to send to all users in channel
    user *sender = ptr_model->getUser(req.senderfd);
    std::string topicMsg = composeMsg("TOPIC", chan->getName(), topic.topic);
    topicMsg = addPrefix(sender->getPrefix(), topicMsg);

    // get all users in channel
    std::vector<user *> channelUsers = ptr_model->getUsersInChannel(chan);
    for (size_t i = 0; i < channelUsers.size(); i++)
        response.insert(std::pair<int, std::string>(channelUsers[i]->getFd(), topicMsg));

    return response;
}