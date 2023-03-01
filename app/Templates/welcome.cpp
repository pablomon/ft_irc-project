#include "welcome.hpp"
#include "msgTools.hpp"
#include "../Services/Server.hpp"

#include <sstream>

std::string welcomeUser(model *ptr_model, user *user, std::string date)
{    
    user->setIsRegistered(true);
    std::string nick = user->getNick();
    std::string msg = composeMsg("001", nick, "Welcome to FT IRC", nick, "!");
    msg.append(composeMsg("002", nick, "Your host is 'The IRCher', running version 1.0"));
    msg.append(composeMsg("003", nick, "This server was created on", date));
    msg.append(composeMsg("004", nick, "FT IRC based of RFC 2119"));

    std::ostringstream ss;
    ss << ptr_model->getUserCount();
    std::string userCount = ss.str();
    msg.append(composeMsg("251", nick, "There are " + userCount + " users"));

    return msg;
};