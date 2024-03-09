#include "Event.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <unistd.h>

cgi_server::Event::Event() {}

cgi_server::Event::~Event()
{
	close(listen_socket_);
}

void	cgi_server::Event::eventloop()
{
	while (true)
	{
		std::cerr << "waiting events..." << "\n";
		waitEvent();
		addActiveEvents();
		callEventHandler();
	}
}

void	cgi_server::Event::waitEvent()
{
	int re = poll(events_.data(), events_.size(), -1);
	if (re == -1)
		std::runtime_error(std::string("poll: ") + strerror(errno));
	std::cerr << re << "event happend" << "\n";
}

void	cgi_server::Event::addEvent(const int fd, short event)
{
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = event;
	this->events_.push_back(pfd);
}

void	cgi_server::Event::deleteEvent(const int fd)
{
	auto result = std::remove_if(events_.begin(), events_.end(), [fd](struct pollfd pfd) {
		return fd == pfd.fd;
	});
	events_.erase(result, events_.end());
}

void	cgi_server::Event::setListenSocket(const int fd)
{
	this->listen_socket_ = fd;
}

void	cgi_server::Event::addActiveEvents()
{
	std::for_each(events_.begin(), events_.end(), [this](struct pollfd pfd) {
		if (isReadEvent(pfd) || isWriteEvent(pfd) || isErrorEvent(pfd))
			this->active_events_.push_back(pfd);
	});
}

void	cgi_server::Event::callEventHandler()
{
	std::for_each(active_events_.begin(), active_events_.end(), [this](struct pollfd pfd) {
		if (isReadEvent(pfd))
			cgi_server::EventHandler::handleReadEvent(pfd.fd, *this);
		else if (isWriteEvent(pfd))
			cgi_server::EventHandler::handleWriteEvent(pfd.fd, *this);
		else if (isErrorEvent(pfd))
			cgi_server::EventHandler::handleErrorEvent(pfd.fd, *this);
	});
}

void	cgi_server::Event::clearAllActiveEvents()
{
	this->active_events_.clear();
}

bool	cgi_server::Event::isReadEvent(const struct pollfd& pfd) const
{
	return pfd.revents & POLLIN || pfd.revents & POLLHUP;
}

bool	cgi_server::Event::isWriteEvent(const struct pollfd& pfd) const
{
	return pfd.revents & POLLOUT;
}

bool	cgi_server::Event::isErrorEvent(const struct pollfd& pfd) const
{
	return pfd.revents & POLLERR || pfd.revents & POLLNVAL;
}

bool	cgi_server::Event::isListenSocket(const int fd) const
{
	return fd == listen_socket_;
}
