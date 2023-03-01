#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <list>
#include "channel.hpp"

#include <iostream>

class user
{
    private:
        std::string username;
        std::string hostname;
        std::string realname;
        std::string ip;
        std::string nick;
        int fd;
        bool isIrcOp;
        bool isRegistered;
        bool hasEnteredPass;

    public:
        user(int fd);
        ~user();

        std::string getUsername();
        void setUsername(std::string username);
        std::string getRealname();
        void setRealname(std::string realname);
        std::string getNick();
        void setNick(std::string nick);
        int getFd();
        bool getIsOperator();
        void setIsOperator(bool isOperator);
        bool getIsRegistered();
        void setIsRegistered(bool isRegistered);
        std::string getHostname();
        void setHostname(std::string hostname);
        std::string getIp();
        void setIp(std::string ip);
        std::string getPrefix();
        bool getPassEntered();
        void setPassEntered(bool value);

        friend std::ostream &operator<<(std::ostream &os, const user &u);
};
    
#endif