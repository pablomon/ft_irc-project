#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include "request.hpp"


class parser
{
public:
    static request parseSingleCommand(int senderfd, const std::string cmd);
    static std::vector<request> parseMessage(int senderfd, const std::string msg);
    static std::string parseMode(std::string mode, channel *chan);
};

#endif // PARSER_HPP