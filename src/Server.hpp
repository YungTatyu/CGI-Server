#ifndef CGI_SERVER_HPP_
#define CGI_SERVER_HPP_

#include "Event.hpp"

#include <string>

namespace cgi_server
{

class Server
{
private:
	Event	event_;
	Server();
public:
	Server(const std::string& address, const unsigned int port);
	~Server();
	void	initListenSocket(const std::string& address, const unsigned int port);
	void	run();
};

} // namespace cgi_server

#endif
