#include "model.hpp"

model::model()
{
    std::cout << "Creating model" << std::endl;
}
model::~model() {}

std::string model::getPassword()
{
    return password;
}

std::string model::setPassword(std::string pwd)
{
    password = pwd;
    return ".";
}

channel *model::getChannel(std::string name)
{
    for (std::vector<channel>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (stringLowcase(it->getName()) == stringLowcase(name))
        {
            return &(*it);
        }
    }
    return NULL;
}

channel *model::newChannel(std::string name)
{
    std::vector<channel>::iterator it;
    for (it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->getName() == name)
        {
            std::cout << "channel already exists" << std::endl;
            return &(*it);
        }
    }

    channel c(name);
    channels.push_back(c);
    return getChannel(name);
}

user *model::newUser(int fd)
{

    std::vector<user>::iterator it;
    for (it = users.begin(); it != users.end(); ++it)
    {
        if (it->getFd() == fd)
        {
            return &(*it);
        }
    }

    user u(fd);
    users.push_back(u);
    return &users.back();
}

user *model::getUser(int fd)
{
    for (std::vector<user>::iterator it = users.begin(); it != users.end(); ++it)
    {
        if (it->getFd() == fd)
        {
            return &(*it);
        }
    }
    return NULL;
}

user *model::getUserByNick(const char *nick)
{
    for (std::vector<user>::iterator it = users.begin(); it != users.end(); ++it)
    {
        if (stringLowcase(it->getNick()) == stringLowcase(nick))
        {
            return &(*it);
        }
    }
    return NULL;
}

void model::removeUser(int userfd)
{

    for (std::vector<class user>::iterator _it1 = users.begin(); _it1 != users.end(); ++_it1)
    {
        if (_it1->getFd() == userfd)
        {
            // get all channels the user is in
            std::vector<channel *> _channels = getChannelsByUser(userfd);

            // remove all channel_users from the user
            for (std::vector<channel *>::iterator _it2 = _channels.begin(); _it2 != _channels.end(); ++_it2)
            {
                removeUserFromChannel(userfd, (*_it2));
            }

            users.erase(_it1);
            std::cout << "User " << userfd << " erased" << std::endl;
            return;
        }
    }
}

int model::getUserCount() { return users.size(); }

channel_user *model::addUserToChannel(int userfd, channel *targetChannel)
{
    std::cout << "Adding user " << userfd << " to channel " << targetChannel->getName() << std::endl;
    for (std::vector<channel_user>::iterator it = channel_users.begin(); it != channel_users.end(); ++it)
    {
        if (it->userfd == userfd && it->channelName == targetChannel->getName())
        {
            std::cout << "User already in channel" << std::endl;
            return &(*it);
        }
    }

    channel_user cu;
    cu.userfd = userfd;
    cu.userLevel = 666;
    cu.isChanop = false;
    cu.channelName = targetChannel->getName();
    channel_users.push_back(cu);

    return &channel_users.back();
}

std::vector<channel *> model::getChannelsByUser(int userfd)
{
    std::vector<channel *> channels;
    for (std::vector<channel_user>::iterator it = channel_users.begin(); it != channel_users.end(); ++it)
    {
        if (it->userfd == userfd)
        {
            channels.push_back(getChannel(it->channelName));
        }
    }
    return channels;
}

std::vector<user *> model::getUsersInChannel(channel *channel)
{

    std::vector<user *> foundUsers;
    std::string channelName = channel->getName();
    int i = 0;
    for (std::vector<channel_user>::iterator it = channel_users.begin(); it != channel_users.end(); ++it)
    {
        if (it->channelName == channelName)
        {
            foundUsers.push_back(getUser(it->userfd));
            i++;
        }
    }
    return foundUsers;
}

void model::removeUserFromChannel(int userfd, channel *channel)
{

    // find channel_user for this user & channel
    std::vector<channel_user>::iterator it = std::vector<channel_user>::iterator();
    for (it = channel_users.begin(); it != channel_users.end(); ++it)
    {
        if (it->userfd == userfd && it->channelName == channel->getName())
            break;
    }

    // remove channel user
    if (it != channel_users.end())
    {
        channel_users.erase(it);
    }

    // remove the channel if it's empty
    std::vector<user *> _users = getUsersInChannel(channel);
    if (getUsersInChannel(channel).size() == 0)
    {
        removeChannel(channel);
    }
}

std::vector<user *> model::removeChannel(channel *channel)
{
    for (std::vector<class channel>::iterator _it = channels.begin(); _it != channels.end(); ++_it)
    {
        if (_it->getName() == channel->getName())
        {
            std::vector<user *> _users = getUsersInChannel(channel);
            for (std::vector<channel_user>::iterator it = channel_users.begin(); it != channel_users.end(); ++it)
            {
                if (it->channelName == channel->getName())
                {
                    channel_users.erase(it);
                }
            }

            channels.erase(_it);
            return _users;
        }
    }
    // std::cout << "Channel " << channel->getName() << " NOT found" << std::endl;
    return std::vector<user *>();
}

std::vector<channel *> model::getChannels()
{
    std::vector<channel *> _channels;
    for (std::vector<class channel>::iterator _it = channels.begin(); _it != channels.end(); ++_it)
    {
        _channels.push_back(&(*_it));
    }
    return _channels;
}

bool model::isUserInChannel(int userfd, channel *channel)
{
    for (std::vector<channel_user>::iterator it = channel_users.begin(); it != channel_users.end(); ++it)
    {
        if (it->userfd == userfd && it->channelName == channel->getName())
            return true;
    }
    return false;
}

channel_user *model::getChannelUser(int userfd, channel *channel)
{
    for (std::vector<channel_user>::iterator _it = channel_users.begin(); _it != channel_users.end(); ++_it)
    {
        if (_it->userfd == userfd && _it->channelName == channel->getName())
        {
            return &(*_it);
        }
    }
    return (NULL);
}

std::string model::getChannelNicks(channel *channel)
{
    std::vector<user *> channelUsers = getUsersInChannel(channel);
    std::string nicks = "";
    for (size_t i = 0; i < channelUsers.size(); i++)
    {
        channel_user *cu = getChannelUser(channelUsers[i]->getFd(), channel);
        if (cu->isChanop)
            nicks += "@";
        nicks += channelUsers[i]->getNick();
        if (i < channelUsers.size() - 1)
            nicks += " ";
    }
    return nicks;
}

std::string model::stringLowcase(std::string str)
{
    std::string res = "";
    char aux;

    for (size_t i = 0; i < str.size(); i++)
    {
        aux = tolower(str[i]);
        res += aux;
    }
    return (res);
}