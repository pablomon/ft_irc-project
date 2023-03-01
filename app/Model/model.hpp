#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include "channel.hpp"
#include "user.hpp"
#include "channel_user.hpp"

#include <iostream>

class model
{
private:
    std::vector<channel> channels;
    std::vector<user> users;
    std::vector<channel_user> channel_users;
    std::string password;

public:
    model();
    ~model();

    std::string setPassword(std::string pwd);
    std::string getPassword();

    user *newUser(int fd);
    user *getUser(int fd);
    user* getUserByNick(const char *nick);
    void removeUser(int fd);
    int getUserCount();

    channel *newChannel(std::string name);
    channel *getChannel(std::string name);
    std::vector<user*> removeChannel(channel *channel);
    std::vector<channel *> getChannels();
    std::vector<channel*> getChannelsByUser(int fd);
    channel_user *addUserToChannel(int fd, channel *channel);
    void removeUserFromChannel(int fd, channel *channel);
    std::vector<user*> getUsersInChannel(channel *channel);
    bool isUserInChannel(int fd, channel *channel);

    channel_user *getChannelUser(int fd, channel *channel);
    std::string getChannelNicks(channel *channel);

    std::string stringLowcase(std::string str);
};

#endif
