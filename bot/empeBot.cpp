#include "empeBot.hpp"

empeBot::empeBot() : bot()
{
    this->_trigger = '!';
    this->_nick = "empe" + this->_nick;
    this->_color = "\033[1;35m";
}

empeBot::empeBot(int port, std::string serverAddr, std::string pass) : bot(port, serverAddr, pass)
{
    this->_trigger = '!';
    this->_nick = "empe" + this->_nick;
    this->_color = "\033[1;35m";
}

empeBot::~empeBot() { }

bool empeBot::help(CmdBot cmd)
{
    std::string msg = "\033[1;37mBOT COMMANDS: \033[0;0m";
    this->sendMsg(cmd.user, msg);
    msg = "\033[1;37m";
    msg += this->_trigger;
    msg += "join <channel>\033[0;0m";
    this->sendMsg(cmd.user, msg);
    msg = "\033[1;37m";
    msg += this->_trigger;
    msg += "leave <channel>\033[0;0m";
    this->sendMsg(cmd.user, msg);
    msg = "\033[1;37m";
    msg += this->_trigger;
    msg += "nick <new_name>\033[0;0m";
    this->sendMsg(cmd.user, msg);
    msg = "\033[1;37m";
    msg += this->_trigger;
    msg += "color <color> <mode>\033[0;0m\n";
    this->sendMsg(cmd.user, msg);
    return 1;
}

void empeBot::botController(CmdBot cmd)
{
    typedef bool (bot::* pfunc)(CmdBot);
    std::map<std::string, pfunc> cmdLib;

    cmdLib["help"] = &bot::help;
    cmdLib["join"] = &bot::joinChannel;
    cmdLib["leave"] = &bot::leaveChannel;
    cmdLib["nick"] = &bot::changeNick;
    cmdLib["color"] = &bot::setColor;

    if (cmd.type == "PRIVMSG")
    {
        if (cmd.args[0][0] == this->_trigger)
        {
            std::string order = &cmd.args[0][1];
            if  (cmdLib.find(order) != cmdLib.end())
                (this->*(cmdLib.at(order)))(cmd);
            else
                this->warningMsg(cmd);
        }
        else
            this->warningMsg(cmd);
    }
    else if (cmd.type == "JOIN")
    {
        sleep(1);
        if (cmd.user != this->_nick)
            this->sendMsg(cmd.args[0] ,"The emperor bless you " + cmd.user + "!");
    }

    return ;
}