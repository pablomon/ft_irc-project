#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#define U_NORMAL 0
#define U_OPERATOR 1
#define U_VOICE 2
#define U_CREATOR 3

#include <string>
#include <list>
#include <algorithm>

# include <iostream>

struct topic
{
    bool isSet;
    std::string topic;
    std::string setter;
    std::string date;
    void clear()
    {
        isSet = false;
        topic = "";
        setter = "";
        date = "";
    }
};

class channel
{
private:
    std::string name;
    std::string creationDate;
    topic topic;
    std::string mode;

public:
    channel(std::string channelName);
    ~channel();
    std::string getName();
    std::string getNameNoPrexi();
    struct topic &getTopic();
    std::string getCreationDate();
    std::string getMode();
};

#endif // CHANNEL
