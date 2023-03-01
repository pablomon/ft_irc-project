#if !defined(CHANNEL_USER_HPP)
#define CHANNEL_USER_HPP

#include "model.hpp"
#include "channel.hpp"
#include "user.hpp"

#define CHANOP_LVL 200
#define VOICE_LVL 100
#define NORMAL_LVL 0

struct channel_user
{
    public:
    int     userfd;

    std::string channelName;
    int     userLevel;
    bool    isChanop;
    std::string userFlags(user *u) {
        std::string flags = "H"; // means user is here, other options not implemented
        if (isChanop)
            flags += "@";
        if (u->getIsOperator())
            flags += "O";
        
        return flags;
    }

};

#endif // CHANNEL-USER
