#include "user.hpp"

user::user(int fd) : fd(fd) 
{
    this->username = "";
    this->realname = "";
    this->hostname = "";
    this->ip = "";
    this->nick = "";
    this->isIrcOp = false;
    this->isRegistered = false;
    this->hasEnteredPass = false;
}

user::~user() {}

std::string user::getUsername() { return username; }
void user::setUsername(std::string username) { this->username = username; }
std::string user::getRealname() { return realname; }
void user::setRealname(std::string realname) { this->realname = realname; }
std::string user::getNick() { return nick; }
void user::setNick(std::string nick) 
{
    // sanitize nick
    for (int i = 0; i < (int)nick.length(); i++)
    {
        if (nick[i] == ' ' || nick[i] == '\t' || nick[i] == '\r' || nick[i] == '\n')
        {
            nick.erase(i, 1);
            i--;
        }
    }
    this->nick = nick; 
}

int user::getFd() { return fd; }
bool user::getIsOperator() { return isIrcOp; }
void user::setIsOperator(bool isOperator) { this->isIrcOp = isOperator; }
bool user::getIsRegistered() { return isRegistered; }
void user::setIsRegistered(bool isRegistered) { this->isRegistered = isRegistered; }
void user::setHostname(std::string hostname) { this->hostname = hostname; }
std::string user::getHostname() { return hostname; }
void user::setIp(std::string ip) { this->ip = ip; }
std::string user::getIp() { return ip; }
std::string user::getPrefix()
{
    std::string prefix;
    if (isIrcOp)
        prefix += "@";
    prefix += nick + "!~" + hostname + "@" + ip;
    return prefix;
}

bool user::getPassEntered() { return hasEnteredPass; }
void user::setPassEntered(bool value) { hasEnteredPass = value; }

std::ostream &operator<<(std::ostream &os, const user &u)
{
    os << "Username: " << u.username << std::endl;
    os << "Realname: " << u.realname << std::endl;
    os << "Nick: " << u.nick << std::endl;
    os << "Fd: " << u.fd << std::endl;
    os << "Is operator: " << u.isIrcOp << std::endl;
    return os;
}