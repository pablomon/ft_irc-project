#include "bot.hpp"

bot::bot() : _port(6667), _serverAddr("127.0.0.1"), _pass(""), _nick("BOT") { this->_color = "\033[0;0m"; this->_eop = 0; }

bot::bot(int port, std::string serverAddr, std::string pass) : _port(port), _serverAddr(serverAddr), _pass(pass), _nick("BOT") { this->_color = "\033[0;0m"; this->_eop = 0; }

bot::~bot() { }

int bot::connectBot()
{
    this->_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (!this->_socket)
    {
        std::cerr << "Could not create socket" << std::endl;
        return (0);
    }
    this->_server.sin_addr.s_addr = inet_addr(this->_serverAddr.c_str());
    this->_server.sin_family = AF_INET;
    this->_server.sin_port = htons(this->_port);
    if (connect(this->_socket, (sockaddr*)&(this->_server), sizeof(this->_server)) < 0)
    {
        std::cerr << "Could not connect to IRC server" << std::endl;
        return (0);
    }
    std::string setter;
    setter = "CAP LS\r\n";
    send(this->_socket, setter.c_str(), setter.size(), 0);

    setter = "PASS " + this->_pass + "\r\n";
    send(this->_socket, setter.c_str(), setter.size(), 0);
    char buffer[512];
    int bytes_received = recv(this->_socket, buffer, 512, 0);
    if (bytes_received > 0)
    {
        std::string message(buffer, bytes_received);
        if (message.find("Password incorrect") != std::string::npos)
        {
            std::cerr << "Wrong server password" << std::endl;
            return (0);
        }
    }
    if (!this->setNick())
    {
        std::cerr << "Could not assing nick name" << std::endl;
        return (0);
    }
    setter = "USER " + this->_nick + " empeBOT botDomain :Bot IRC_42Madrid\r\n";
    send(this->_socket, setter.c_str(), setter.size(), 0);

    return (1);
}

void bot::execute()
{
    while (!this->_eop)
    {
        char buffer[512];
        int bytes_received = recv(this->_socket, buffer, 512, 0);
        if (bytes_received > 0)
        {
            std::string message(buffer, bytes_received);

            /*  TEXT DISPLAY */
            std::cout << message << std::endl;
            /*****************/

            if (message.find("PING :") == 0)
            {
                this->rBeacon(message);
                return ;
            }
            this->botController(this->splitter(message));
        }
    }
}

CmdBot bot::splitter(std::string msg)
{
    int index = 0;
    int start;
    CmdBot cmdBot;
    
    if (msg[index] == ':')
    {
        start = ++index;
        while (msg[index] != '!')
            index++;
        cmdBot.user = msg.substr(start, index - start);
        while (msg[index] != ' ')
            index++;
        start = ++index;
        while (msg[index] != ' ')
            index++;
        cmdBot.type = msg.substr(start, index - start);
        start = ++index;
        if (cmdBot.type == "PRIVMSG")
        {
            while (msg[index] != ' ')
                index++;
            cmdBot.target = msg.substr(start, index - start);
        }
        else
        {
            cmdBot.target = "NONE";
        }
        while (msg[index] != ':')
                index++;
        start = ++index;
        while (msg[index] != '\r')
        {
            if (msg[index] == ' ')
            {
                cmdBot.args.push_back(msg.substr(start, index - start));
                start = index + 1;
            }
            index++;
        }
        cmdBot.args.push_back(msg.substr(start, index - start));
    }
    else
    {
        cmdBot.user = "SERVER";
        cmdBot.type = "SERVER_INFO";
        cmdBot.target = "NONE";
        cmdBot.args.push_back("NONE");
    }

    /*  DISPLAY COMMANDS */
    // std::cout << "---> BOT COMMAND DISPLAY <---" << std::endl;
    // std::cout << "USER: " << cmdBot.user << std::endl;
    // std::cout << "TYPE: " << cmdBot.type << std::endl;
    // std::cout << "TARGET: " << cmdBot.target << std::endl;
    // for (size_t i = 0; i < cmdBot.args.size(); i++)
    //     std::cout << "ARGS: " << cmdBot.args.at(i) << std::endl;
    // std::cout << "---> END OF DISPLAY <---" << std::endl;
    //**********//

    return (cmdBot);
}

void bot::rBeacon(std::string message)
{
    std::string msgCmd = "PONG " + message.substr(6) + EOMSG;
    send(this->_socket, msgCmd.c_str(), msgCmd.size(), 0);
}

void bot::warningMsg(CmdBot cmd)
{
    this->sendMsg(cmd.user, "Incorrect commands for bot (" + this->_nick + ").\r\n");
    this->help(cmd);
}

bool bot::joinChannel(CmdBot cmd)
{
    std::string msgCmd = "JOIN :";
    for (std::vector<std::string>::iterator it = cmd.args.begin() + 1; it != cmd.args.end(); ++it)
    {
        if ((*it)[0] != '#')
            msgCmd += "#" + *it;
        else
            msgCmd += *it;
        if (it + 1 != cmd.args.end())
            msgCmd += " ";
    }
    msgCmd += EOMSG;
    send(this->_socket, msgCmd.c_str(), msgCmd.size(), 0);
    return 1;
}

bool bot::leaveChannel(CmdBot cmd)
{
    std::string msgCmd = "PART :";
    for (std::vector<std::string>::iterator it = cmd.args.begin() + 1; it != cmd.args.end(); ++it)
    {
        if ((*it)[0] != '#')
            msgCmd += "#" + *it;
        else
            msgCmd += *it;
        if (it + 1 != cmd.args.end())
            msgCmd += " ";
    }
    msgCmd += EOMSG;
    send(this->_socket, msgCmd.c_str(), msgCmd.size(), 0);
    return 1;
}

bool bot::changeNick(CmdBot cmd)
{
    std::string nick;

    nick = cmd.args[1];
    std::string msgCmd = "NICK " + nick + EOMSG;
    send(this->_socket, msgCmd.c_str(), msgCmd.size(), 0);
    char buffer[512];
    int bytes_received = recv(this->_socket, buffer, 512, 0);
    std::string message(buffer, bytes_received);
    if (bytes_received > 0)
    {
        if (message.find("NICK :" + nick) != std::string::npos)
        {
            this->sendMsg(cmd.user, "The nick changed from " + this->_nick + " to " + nick);
            this->_nick = nick;
            return 1;
        }
        else
            this->sendMsg(cmd.user, "The nick is already taken. Try another.");
    }
    return 0;
}

bool bot::setNick()
{
    int i = 0;
    std::string nick = this->_nick;

    while (nick != "")
    {
        std::string msgCmd = "NICK " + nick + EOMSG;
        send(this->_socket, msgCmd.c_str(), msgCmd.size(), 0);

        char buffer[512];
        int bytes_received = recv(this->_socket, buffer, 512, 0);
        std::string message(buffer, bytes_received);
        if (bytes_received > 0)
        {
            if (message.find("NICK :" + nick) != std::string::npos)
            {
                this->_nick = nick;
                return 1;
            }
            else
                nick += "_";
        }
        i++;
        if (i == 3)
            nick = "";
    }
    return 0;
}

bool bot::setColor(CmdBot cmd)
{
    std::string color = cmd.args[1];
    std::string mode = "0";

    if (cmd.args.size() > 2)
        std::string mode = cmd.args[2];
    this->_color = "\033[";
    this->_color += mode;
    this->_color += ";";
    this->_color += color;
    this->_color += "m";
    return 1;
}

void bot::close() { this->_eop = 1; }

void bot::sendMsg(std::string target, std::string msg)
{
    std::string msgCmd = "PRIVMSG " + target + " :" + this->_color + msg + C_RESET + EOMSG;
    std::cout << "BOT_MSG to SERVER===>  " + msgCmd;
    send(this->_socket, msgCmd.c_str(), msgCmd.size(), 0);
}