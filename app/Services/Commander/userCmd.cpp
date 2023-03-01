
#include "userCmd.hpp"
#include "../../Templates/welcome.hpp"
#include <sstream>

userCmd::userCmd(model *ptr_model, request req, std::string creationDate) : command(ptr_model, req) {
    _creationDate = creationDate;
}
userCmd::~userCmd() {}

std::map<int, std::string> userCmd::execute()
{
    // Comprueba que el número de parametros sea correcto
    if (req.args.size() < 4)
    {
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("461", "Not enough parameters")));
        return response;
    }
    // Comprueba si el usuario ya tiene un nombre de usuario
    user *u = ptr_model->getUser(req.senderfd);
    if (u->getUsername() != "")
    {
        // Si ya tiene un nombre de usuario, envía un 462
        std::cout << "User " << u->getNick() << " already has a username, sending a 462" << std::endl;
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("462", ":You may not reregister")));
        return response;
    }
    std::cout << "User " << u->getNick() << " has been assigned a username" << std::endl;
    // Asigna el nombre de usuario
    u->setUsername(req.args[0]);
    // asigna el hostname
    u->setHostname(req.args[1]);
    // asigna el ip
    u->setIp(req.args[2]);
    // Asigna el nombre real
    u->setRealname(req.args[3]);

    // Si el usuario ya tiene un nick, envía el mensaje de bienvenida
    if (u->getNick() != "")
    {
        std::cout << "User has nick, sending welcome message" << std::endl;
        response.insert(std::pair<int, std::string>(req.senderfd, welcomeUser(ptr_model, u, _creationDate)));
    }

    return response;
}
