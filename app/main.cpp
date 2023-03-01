#include "Services/Server.hpp"

int main (int argc, char **argv)
{
    std::string pass;
    try
    {
        if (argc != 3)
            throw std::runtime_error("./ircserv <port> <password>");
        pass = argv[2];        
        Server Server("127.0.0.1", argv[1], pass);
    }
    catch(const std::exception& e)
    {
        std::cerr << "struct comand: " << e.what() << std::endl;
    }    

    return (0);
}
