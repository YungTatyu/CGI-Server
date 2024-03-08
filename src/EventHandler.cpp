#include "EventHandler.hpp"

#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>

void	cgi_server::EventHandler::handleReadEvent(const int fd, Event& event)
{
	if (event.isListenSocket(fd))
	{
		acceptNewClient(fd, event);
	}
}

void	cgi_server::EventHandler::acceptNewClient(const int fd, Event& event)
{
	int cli_sock = accept(fd, NULL, NULL);
	if (cli_sock == -1)
		std::cerr << "accept:" << strerror(errno) << std::endl;

	event.addEvent(cli_sock, POLLIN);
}

void	cgi_server::EventHandler::handleWriteEvent(const int fd, Event& event)
{
	const std::string body = "<p>this is cgi-made response</p>";

	// cgiはresponseを標準出力に出力する
	std::cout << "content-type" << "200\n"
		<< "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title>Document</title></head><body>"
		<< body << "</body></html>" << std::endl;

	close(fd);
	event.deleteEvent(fd);
}

void	cgi_server::EventHandler::handleErrorEvent(const int fd, Event& event)
{
	close(fd);
	event.deleteEvent(fd);
}
