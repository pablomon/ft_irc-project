#if !defined(MSG_TOOLS_HPP)
#define MSG_TOOLS_HPP

#include <vector>
#include <map>
#include <string>

std::string composeMsg(std::string cmd, std::string p1);
std::string composeMsg(std::string cmd, std::string p1, std::string p2);
std::string composeMsg(std::string cmd, std::string p1, std::string p2, std::string p3);
std::string composeMsg(std::string cmd, std::string p1, std::string p2, std::string p3, std::string p4);
std::string addPrefix(std::string prefix, std::string msg);
std::vector<std::string> split(const std::string &str, const std::string &delim);

#endif // MSG_TOOLS_HPP
