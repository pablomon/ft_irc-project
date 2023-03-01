#ifndef EMPEBOT_HPP
#define EMPEBOT_HPP

#include "bot.hpp"

class empeBot : public bot
{
    public:
        empeBot();
        empeBot(int port, std::string serverAddr, std::string pass);
        ~empeBot();

        bool help(CmdBot cmd);

        void botController(CmdBot cmd);
};
#endif
