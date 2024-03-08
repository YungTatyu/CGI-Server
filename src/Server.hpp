#ifndef CGI_SERVER_HPP_
#define CGI_SERVER_HPP_

#include "Event.hpp"

#include <string>

namespace cgi_server
{

class Server
{
private:
	Event	event;
	Server();
public:
	Server(const std::string& address, const unsigned int port);
	~Server();
	void	initListenSocket(const std::string& address, const unsigned int port);
};

} // namespace cgi_server

#endif
