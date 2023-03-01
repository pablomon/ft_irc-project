#include "parser.hpp"

#include <algorithm>

#define DEBUG 0

request parser::parseSingleCommand(int senderfd, const std::string cmd)
{
    request req;
    if (DEBUG) std::cout << "Parsing command: " << cmd << std::endl;

    // Split the command into space-separated fields
    std::vector<std::string> splitCommand;
    std::size_t start = 0;
    std::size_t end = cmd.find(" ");
    while (end != std::string::npos)
    {
        std::string field = cmd.substr(start, end - start);
        if (field[0] == ':') // if the field starts with a colon its the last parameter
        {
            if (DEBUG) std::cout << "Last parameter found" << std::endl;
            start++;
            break;
        }
        splitCommand.push_back(cmd.substr(start, end - start));
        start = end + 1;
        end = cmd.find(" ", start);
    }

    std::string lastField = cmd.substr(start, std::string::npos - start);
    if (lastField[0] == ':') // if the field starts with a colon its the last parameter
        start++;
    splitCommand.push_back(cmd.substr(start, std::string::npos - start));

    // splitCommand.push_back(cmd.substr(start));
    // The first field is the command itself
    std::string cmdStr = splitCommand[0];
    std::transform(cmdStr.begin(), cmdStr.end(),cmdStr.begin(), ::toupper);
    req.command = cmdStr;

    // Parse the rest of the fields
    std::size_t i = 1;
    while (i < splitCommand.size())
    {
        std::string field = splitCommand[i];
        req.args.push_back(field);
        i++;
    }

    if (DEBUG) std::cout << "request:" << std::endl;
    if (DEBUG) std::cout << "command: " << req.command << std::endl;
    for (std::vector<std::string>::iterator it = req.args.begin(); it != req.args.end(); ++it)
        if (DEBUG) std::cout << "arg: " << *it << std::endl;

    req.senderfd = senderfd;
    return req;
}

std::vector<request> parser::parseMessage(int senderfd, const std::string msg)
{
    std::vector<request> reqs;
    int _numCmds = 0;

    if (msg.empty())
        return std::vector<request>(); // return a null vector

    std::vector<std::string> cmds;
    std::size_t start = 0;
    std::size_t end = msg.find("\r\n");
    while (end != std::string::npos)
    {
        if (DEBUG) std::cout << "pushing back: " << msg.substr(start, end - start) << std::endl;
        cmds.push_back(msg.substr(start, end - start));
        start = end + 2;
        end = msg.find("\r\n", start);

        _numCmds++;
    }

    if (DEBUG) std::cout << "Number of commands: " << _numCmds << std::endl;
    for (std::vector<std::string>::iterator it = cmds.begin(); it != cmds.end(); ++it)
        reqs.push_back(parseSingleCommand(senderfd, *it));

    return reqs;
}

std::string parser::parseMode(std::string mode, channel *chan)
{
    std::cout << "Parsing mode: " << mode << "channel: " << chan << std::endl;
    return "";
}