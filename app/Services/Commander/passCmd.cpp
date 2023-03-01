#include "passCmd.hpp"

passCmd::passCmd(model *ptr_model, request req, Server *ptr_server) : command(ptr_model, req)
{
	_ptr_server = ptr_server;
}

passCmd::~passCmd() {}

std::map<int, std::string> passCmd::execute()
{
	// args num check
	if (req.args.size() == 0)
	{
		response.insert(std::pair<int, std::string>(req.senderfd, composeMsg("461", "Need more parameters")));
		return response;
	}

	user *u = ptr_model->getUser(req.senderfd);
	if (req.args[0] != _ptr_server->getPass())
	{
		std::string msg = composeMsg("464", u->getNick(), "Password incorrect");
		response.insert(std::pair<int, std::string>(req.senderfd, msg));
		_ptr_server->mark4kill(req.senderfd);
		return response;
	}

	u->setPassEntered(true);
	std::string msg = composeMsg("NOTICE", "", "Password ok");
	response.insert(std::pair<int, std::string>(req.senderfd, msg));

	return response;
}
