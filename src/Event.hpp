#ifndef CGI_SERVER_EVENT_HPP_
#define CGI_SERVER_EVENT_HPP_

#include <poll.h>
#include <vector>

namespace cgi_server
{
class Event
{
private:
	int	listen_socket;
	std::vector<struct pollfd>	events;
public:
	Event();
	~Event();
	void	eventloop();
	void	addEvent(const int fd, short event);
	void	deleteEvent();
	void	setListenSocket(const int fd);
};	
} // namespace cgi_server

#endif
