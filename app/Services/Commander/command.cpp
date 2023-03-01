#include "command.hpp"

command::command(model *ptr_model, request req) : ptr_model(ptr_model), req(req) { }
command::~command() {}

std::map<int, std::string> command:: execute()
{
    return response;
}