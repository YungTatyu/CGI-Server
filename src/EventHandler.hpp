#ifndef CGI_SERVER_EVENT_HANDLER_HPP_
#define CGI_SERVER_EVENT_HANDLER_HPP_

#include "Event.hpp"
#include <string>

namespace cgi_server
{
class Event;
class EventHandler
{
private:
	EventHandler();
public:
	~EventHandler();
	static void	handleReadEvent(const int fd, Event& event);
	static void	handleWriteEvent(const int fd, Event& event);
	static void	handleErrorEvent(const int fd, Event& event);
	static void	acceptNewClient(const int fd, Event& event);
	static void	recvRequest(const int fd, Event& event);
	static void	sendResponse(const int fd, Event& event, const std::string& response);
};	
} // namespace cgi_server

#endif
