#ifndef CGI_SERVER_EVENT_HPP_
#define CGI_SERVER_EVENT_HPP_

#include "EventHandler.hpp"

#include <poll.h>
#include <vector>

namespace cgi_server
{
class Event
{
private:
	int	listen_socket_;
	std::vector<struct pollfd>	events_;
	std::vector<struct pollfd>	active_events_;
	void	waitEvent();
	void	addActiveEvents();
	void	callEventHandler();
	void	clearAllEvents();
	void	clearAllActiveEvents();
	bool	isReadEvent(const struct pollfd& pfd) const;
	bool	isWriteEvent(const struct pollfd& pfd) const;
	bool	isErrorEvent(const struct pollfd& pfd) const;
public:
	Event();
	~Event();
	void	eventloop();
	void	addEvent(const int fd, short event);
	void	updateEvent(const int fd, short event);
	void	deleteEvent(const int fd);
	void	setListenSocket(const int fd);
	bool	isListenSocket(const int fd) const;
};	
} // namespace cgi_server

#endif
