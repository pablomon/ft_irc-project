#include "channel.hpp"

#include <ctime>
#include <sstream>

channel::channel(std::string channelName) : name(channelName) {
    // get current time
    time_t now = time(0);
    // convert now to string form
    std::ostringstream ss;
    ss << now;
    creationDate = ss.str();
    mode = "";

    std::cout << "Channel created with name " << name << std::endl;
}

channel::~channel() {}

std::string channel::getName() { return name; }
std::string channel::getNameNoPrexi() { return name.substr(1); }
struct topic &channel::getTopic() { return topic; }
std::string channel::getCreationDate() { return creationDate; }
std::string channel::getMode() { return mode; }
