#ifndef REQUEST_HPP
#define REQUEST_HPP


#include <string>
#include <vector>
#include "../Model/user.hpp"

struct request
{
    public:
    int senderfd;
    std::string prefix;
    std::string command;
    std::vector<std::string> args;
    std::string trailing;
    
    std::string showContent()
    {
        std::string content = "command: " + command + "\r\n";                        
        for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it)
        {
            content.append("arg: " + *it + "\r\n");
        }
        return content;
    }
};



#endif // REQUEST_HPP
