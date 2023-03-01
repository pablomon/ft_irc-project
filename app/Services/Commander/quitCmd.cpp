#include "quitCmd.hpp"

quitCmd::quitCmd(model *ptr_model, request req) : command(ptr_model, req) {}
quitCmd::~quitCmd() {}

std::map<int, std::string> quitCmd::execute()
{
    // check args
    if (req.args.size() < 1)
        req.args.push_back("not provided");

    // send quit message to channel
    user *u = ptr_model->getUser(req.senderfd);
    std::vector<channel *> channels = ptr_model->getChannelsByUser(req.senderfd);
    std::string quitMessage = composeMsg("QUIT", u->getNick(), req.args[0]);
    quitMessage = addPrefix(u->getPrefix(), quitMessage);
    for (std::vector<channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        std::vector<user *> channelUsers = ptr_model->getUsersInChannel(*it);
        for (std::vector<user *>::iterator it2 = channelUsers.begin(); it2 != channelUsers.end(); ++it2)
        {
            if ((*it2)->getFd() != req.senderfd)
            {
                response.insert(std::pair<int, std::string>((*it2)->getFd(), quitMessage));
            }
        }
    }

    // add error message to sender to indicate that the connection is closed
    response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("ERROR", "Closing Link: " + u->getNick() + " (" + req.args[0] + ")")));
    return response;
}
