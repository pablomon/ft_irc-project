#include "listCmd.hpp"

listCmd::listCmd(model *ptr_model, request req) : command(ptr_model, req) {}

listCmd::~listCmd() {}

std::string listCmd::listDisplay(channel *ch, int nUsers)
{
    std::string listElem = ch->getName() + " ";
    std::stringstream ss;

    ss << nUsers;
    listElem.append(ss.str() + " ");
    ss.flush();
    listElem.append("[" + ch->getMode() + "]");
    if ((ch->getTopic()).isSet == true)
        listElem.append(" " + (ch->getTopic()).topic);
    return listElem;
}

std::map<int, std::string> listCmd::execute()
{
    std::vector<channel *> aux = this->ptr_model->getChannels();
    std::string listMsg = "";
    
    for (std::vector<channel *>::iterator it = aux.begin(); it != aux.end(); ++it)
    {
        int nUsers = this->ptr_model->getUsersInChannel(*it).size();
        listMsg.append(this->listDisplay(*it, nUsers) + "\r\n");
    }
    listMsg.append("End of /LIST\r\n");
    response.insert(std::pair<int, std::string>(this->req.senderfd, listMsg));
    return response;
}