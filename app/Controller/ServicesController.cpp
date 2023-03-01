
#include "ServicesController.hpp"

#include "../Templates/parser.hpp"
#include "../Services/Commander/command.hpp"
#include "../Services/Commander/nickCmd.hpp"
#include "../Services/Commander/passCmd.hpp"
#include "../Services/Commander/userCmd.hpp"
#include "../Services/Commander/pingCmd.hpp"
#include "../Services/Commander/joinCmd.hpp"
#include "../Services/Commander/privmsgCmd.hpp"
#include "../Services/Commander/quitCmd.hpp"
#include "../Services/Commander/partCmd.hpp"
#include "../Services/Commander/topicCmd.hpp"
#include "../Services/Commander/kickCmd.hpp"
#include "../Services/Commander/noticeCmd.hpp"
#include "../Services/Commander/modeCmd.hpp"
#include "../Services/Commander/whoCmd.hpp"
#include "../Services/Commander/killCmd.hpp"
#include "../Services/Commander/operCmd.hpp"
#include "../Services/Commander/listCmd.hpp"
#include "../Services/Commander/dieCmd.hpp"

#include "../Services/Server.hpp"

#include <vector>
#include <map>

ServicesController::ServicesController(void) { _ptr_server = NULL; }
ServicesController::~ServicesController(void) { }

void ServicesController::setServer(Server *ptr) { 
    _ptr_server = ptr;
    _creationDate = ptr->getCreationDate();
}

std::map<int, std::string> mapAppend(std::map<int, std::string> &base, std::map<int, std::string> &appendice)
{
    for (std::map<int, std::string>::iterator it = appendice.begin(); it != appendice.end(); ++it)
    {
        if (base.find(it->first) != base.end())
            base[it->first] += it->second;
        else
            base[it->first] = it->second;
    }
    return base;
}

std::map<int, std::string> ServicesController::getCommandResponse(int senderfd, std::string msg)
{
    _response.clear();    
    _requests.clear();
    _requests = parser::parseMessage(senderfd, msg);

    for (std::vector<request>::iterator it = _requests.begin(); it != _requests.end(); ++it)
    {
        command *cmd = NULL;
        user *u = _model.getUser((*it).senderfd);
        if (it->command != "CAP" && it->command != "PASS" && !u->getPassEntered())
            _ptr_server->mark4kill((*it).senderfd);
            
        if (it->command == "PASS")
            cmd = new passCmd(&_model, *it, _ptr_server);
        if (it->command == "NICK")
            cmd = new nickCmd(&_model, *it, _creationDate);
        if (it->command == "USER")
            cmd = new userCmd(&_model, *it, _creationDate);
        if (it->command == "PING")
            cmd = new pingCmd(&_model, *it);
        if (it->command == "JOIN")
            cmd = new joinCmd(&_model, *it);
        if (it->command == "PRIVMSG")
            cmd = new privmsgCmd(&_model, *it);
        if (it->command == "PART")
            cmd = new partCmd(&_model, *it);
        if (it->command == "TOPIC")
            cmd = new topicCmd(&_model, *it);
        if (it->command == "KICK")
            cmd = new kickCmd(&_model, *it);
        if (it->command == "NOTICE")
            cmd = new noticeCmd(&_model, *it);
        if (it->command == "MODE")
            cmd = new modeCmd(&_model, *it);
        if (it->command == "WHO")
            cmd = new whoCmd(&_model, *it);
        if (it->command == "OPER")
            cmd = new operCmd(&_model, *it);
        if (it->command == "LIST")
            cmd = new listCmd(&_model, *it);
        if (it->command == "KILL")
            cmd = new killCmd(&_model, *it, _ptr_server);        
        if (it->command == "QUIT")
        {
            _ptr_server->mark4kill((*it).senderfd);
            cmd = new quitCmd(&_model, *it);
        }
        if (it->command == "DIE")
            cmd = new dieCmd(&_model, *it, _ptr_server);
        
        if (cmd)
        {
            std::map<int, std::string> cmdres = cmd->execute();
            _response = mapAppend(_response, cmdres);
            delete(cmd);
        }
        
    }
    return _response;
}

void ServicesController::registerUser(int userFd)
{
    _model.newUser(userFd);
}

void ServicesController::deregisterUser(int userFd)
{
    _model.removeUser(userFd);
}