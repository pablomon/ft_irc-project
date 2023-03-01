#include "msgTools.hpp"

#include <vector>
#include <map>
#include <string>

std::string composeMsg(std::string cmd, std::string p1)
{
    std::string res;
    // if (!source.empty())
    //     res = ":" + source + " ";
    res += cmd + " ";
    res += ":" + p1;
    res += "\r\n";
    return res;
}

std::string composeMsg(std::string cmd, std::string p1, std::string p2)
{
    std::string res;
    // if (!source.empty())
    //     res = ":" + source + " ";
    res += cmd + " ";
    res += p1 + " ";
    res += ":" + p2;
    res += "\r\n";
    return res;
}
std::string composeMsg(std::string cmd, std::string p1, std::string p2, std::string p3)
{
    std::string res;
    // if (!source.empty())
    //     res = ":" + source + " ";
    res += cmd + " ";
    res += p1 + " ";
    res += p2 + " ";
    res += ":" + p3;
    res += "\r\n";
    return res;
}
std::string composeMsg(std::string cmd, std::string p1, std::string p2, std::string p3, std::string p4)
{
    std::string res;
    // if (!source.empty())
    //     res = ":" + source + " ";
    res += cmd + " ";
    res += p1 + " ";
    res += p2 + " ";
    res += p3 + " ";
    res += ":" + p4;
    res += "\r\n";
    return res;
}
std::string addPrefix(std::string prefix, std::string msg)
{
    std::string res;
    res = ":" + prefix + " ";
    res += msg;
    return res;
}

std::vector<std::string> split(const std::string &str, const std::string &delim)
{
    std::vector<std::string> fields;
    std::size_t start = 0;
    std::size_t end = str.find(delim);
    while (end != std::string::npos)
    {
        fields.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delim, start);
    }
    fields.push_back(str.substr(start, std::string::npos - start));
    return fields;
}