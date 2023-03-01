#include "modeCmd.hpp"
#include "../../Templates/parser.hpp"

modeCmd::modeCmd(model *ptr_model, request req) : command(ptr_model, req) {}
modeCmd::~modeCmd() {}
std::map<int, std::string> modeCmd::execute()
{
    // args num check
    if (req.args.size() == 0)
    {
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("461", "Need more parameters")));
        return response;
    }

    // check target
    if (req.args[0][0] == '#')
        doChannelMode();
    else
        doUserMode();

    return response;
}

void modeCmd::doUserMode()
{
}

void modeCmd::doChannelMode()
{
    // check if channel exists
    channel *chan = ptr_model->getChannel(req.args[0]);
    if (chan == NULL)
    {
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("403", ptr_model->getUser(req.senderfd)->getNick(), "No such channel: " + req.args[0])));
        return;
    }
    // check if user is in channel
    if (!ptr_model->isUserInChannel(req.senderfd, chan))
    {
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("442", ptr_model->getUser(req.senderfd)->getNick(), "You're not on that channel: " + chan->getName())));
        return;
    }
    // check whether mode is given
    if (req.args.size() == 1)
    {
        // gather channel mode
        std::string msg = composeMsg("324", ptr_model->getUser(req.senderfd)->getNick(), chan->getName(), chan->getMode(), "");
        // add creation date
        msg.append(composeMsg("329", ptr_model->getUser(req.senderfd)->getNick(), chan->getName(), chan->getCreationDate()));
        response.insert(std::pair<int, std::string>(req.senderfd, msg));
        return;
    }
    else if (req.args.size() == 3)
    {
        user *u = ptr_model->getUser(req.senderfd);
        // check if user is chanop
        if (!ptr_model->getChannelUser(req.senderfd, chan)->isChanop && !u->getIsOperator())
        {
            response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("482", ptr_model->getUser(req.senderfd)->getNick(), "You're not channel operator")));
            return;
        }
        // check if mode is valid
        if (req.args[1] != "+o" && req.args[1] != "-o")
        {
            response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("472", ptr_model->getUser(req.senderfd)->getNick(), "Unknown mode")));
            return;
        }
        // check if target is valid
        user *target = ptr_model->getUserByNick(req.args[2].c_str());
        if (target == NULL)
        {
            response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("401", ptr_model->getUser(req.senderfd)->getNick(), "No such nick: " + req.args[2])));
            return;
        }
        // check if target is in channel
        if (!ptr_model->isUserInChannel(target->getFd(), chan))
        {
            response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("441", ptr_model->getUser(req.senderfd)->getNick(), "Not on that channel: " + req.args[2])));
            return;
        }

        //  check if target is already chanop
        if (req.args[1] == "+o" && ptr_model->getChannelUser(target->getFd(), chan)->isChanop)
        {
            response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("482", ptr_model->getUser(req.senderfd)->getNick(), "User is already channel operator")));
            return;
        }
        // check if target is not chanop
        if (req.args[1] == "-o" && !ptr_model->getChannelUser(target->getFd(), chan)->isChanop)
        {
            response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("482", ptr_model->getUser(req.senderfd)->getNick(), "User is not channel operator")));
            return;
        }
        // set chanop
        ptr_model->getChannelUser(target->getFd(), chan)->isChanop = (req.args[1] == "+o");
        // add message for channel users
        std::string msg = composeMsg("MODE", chan->getName(), req.args[1], req.args[2]);
        std::vector<user *> users = ptr_model->getUsersInChannel(chan);
        for (std::vector<user *>::iterator it = users.begin(); it != users.end(); it++)
            response.insert(std::pair<int, std::string>((*it)->getFd(), msg));
    }
    else
    {
        // check ban list
        if (req.args[1] == "b")
        {
            std::string msg = composeMsg("368", ptr_model->getUser(req.senderfd)->getNick(), chan->getName(), "End of channel ban list");
            response.insert(std::pair<int, std::string>(req.senderfd, msg));
            return;
        }
    }
}