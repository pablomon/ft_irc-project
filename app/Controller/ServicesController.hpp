#ifndef SERVICE_CONTROLLER_HPP
#define SERVICE_CONTROLLER_HPP
#include <iostream>
#include <string>
#include "../Model/model.hpp"
#include "../Templates/request.hpp"
class Server;
#include <map>
struct Command
{
    std::string     fullCommand;
    std::string     commandType;
};
class ServicesController
{
    private:
        std::string _buffer;
        model _model;
        Server *_ptr_server;
        std::map<int, std::string> _response;
        std::vector<request> _requests;
        std::string _creationDate;

    public:
        ServicesController( void );
        ~ServicesController( void );
        void setServer(Server *ptr);
        std::map<int, std::string> getCommandResponse(int userfd, std::string buffer);
        void registerUser(int userfd);
        void deregisterUser(int userfd);
};
#endif