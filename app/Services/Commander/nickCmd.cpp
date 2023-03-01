#include "nickCmd.hpp"
#include "../../Templates/rule.hpp"
#include "../../Templates/welcome.hpp"

nickCmd::nickCmd(model *ptr_model, request req, std::string creationDate) : command(ptr_model, req)
{
    _creationDate = creationDate;
}
nickCmd::~nickCmd() {}

std::map<int, std::string> nickCmd::execute()
{
    // args num check
    if (req.args.size() == 0)
    {
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("461", "Need more parameters")));
        return response;
    }

    if (!validateNickname(req.args[0].c_str()))
    {
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("432", ":Erroneous nickname is no correct!!")));
        return response;
    }

    // // Comprueba que el nick no esté vacio
    if (req.args.size() == 0)
    {
        std::cout << "Nick is empty, sending a 431" << std::endl;
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("431", "No nickname given")));
        return response;
    }
    
    // check if nick is already in use
    user *u = ptr_model->getUserByNick(req.args[0].c_str());
    if (u != NULL)
    {
        response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("433", "*", req.args[0], "Nickname is already in use")));
        std::cout << "Nick " << req.args[0] << " already in use, send a 433" << std::endl;
        return response;
    }

    // si el usuario ya tiene un nick, cambiamos el nick
    u = ptr_model->getUser(req.senderfd);
    if (u->getNick() != "")
    {
        std::cout << "User " << u->getNick() << " already has a nick, changing it to " << req.args[0] << std::endl;
        std::string prefix = u->getPrefix();
        u->setNick(req.args[0]);

        // compon el mensaje de cabmio de nick
        std::string nickMsg = composeMsg("NICK", req.args[0]);
        nickMsg = addPrefix(prefix, nickMsg);
        // obten todos los usuarios que están en los canales en los que está el usuario
        std::vector<channel *> channels = ptr_model->getChannelsByUser(u->getFd());
        for (std::vector<channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
        {
            std::vector<user *> channelUsers = ptr_model->getUsersInChannel((*it));
            for (std::vector<user *>::iterator it2 = channelUsers.begin(); it2 != channelUsers.end(); ++it2)
                response.insert(std::pair<int, std::string>((*it2)->getFd(), nickMsg));
        }
        // añade al propio usuario
        response.insert(std::pair<int, std::string>(u->getFd(), nickMsg));
        // envía el mensaje a todos los usuarios
        return response;
    }

    // si el usuario no tiene nick, se lo asignamos
    u->setNick(req.args[0]);
    std::cout << "fd " << u->getFd() << " has been assigned a nick: " << u->getNick() << std::endl;

    std::string msg = composeMsg("NICK", req.args[0]);
    msg = addPrefix(u->getPrefix(), msg);

    // si además el usuario ya está registrado, enviamos el mensaje de bienvenida
    if (u->getUsername() != "")
    {
        std::cout << "User " << u->getNick() << " is already registered, sending welcome messages" << std::endl;
        std::string wmsg = welcomeUser(ptr_model, u, _creationDate);
        msg.append(wmsg);
    }
    else
        std::cout << "User " << u->getNick() << " is not registered yet" << std::endl;

    response.insert(std::pair<int, std::string>(u->getFd(), msg));

    return response;
}