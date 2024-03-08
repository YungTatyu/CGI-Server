#include "Event.hpp"

cgi_server::Event::Event() {}

cgi_server::Event::~Event() {}

void	cgi_server::Event::eventloop()
{

}

void	cgi_server::Event::addEvent(const int fd, short event)
{
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = event;

	this->events.push_back(pfd);
}

void	cgi_server::Event::setListenSocket(const int fd)
{
	this->listen_socket = fd;
}
