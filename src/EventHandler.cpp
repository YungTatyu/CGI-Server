#include "EventHandler.hpp"

#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iterator>

void	cgi_server::EventHandler::handleReadEvent(const int fd, Event& event)
{
	if (event.isListenSocket(fd))
	{
		acceptNewClient(fd, event);
		return;
	}
	recvRequest(fd, event);
}

void	cgi_server::EventHandler::acceptNewClient(const int fd, Event& event)
{
	int cli_sock = accept(fd, NULL, NULL);
	if (cli_sock == -1)
		std::cerr << "accept:" << strerror(errno) << std::endl;

	event.addEvent(cli_sock, POLLIN);
}

void	cgi_server::EventHandler::recvRequest(const int fd, Event& event)
{
	const unsigned int	buffersize = 1024;
	std::vector<unsigned char>	request;
	ssize_t	total_read_bytes = 0;
	while (true)
	{
		request.resize(request.size() + buffersize);
		ssize_t read_bytes = recv(fd, request.data() + total_read_bytes, buffersize, 0);
		if (read_bytes == -1)
		{
			std::cerr << "recv: " << strerror(errno);
			return;
		}
		if (read_bytes < buffersize)
			break;
		total_read_bytes += read_bytes;
	}

	std::cerr << "request:\n";
	std::copy(request.begin(), request.end(), std::ostream_iterator<unsigned char>(std::cerr, " "));
	std::cerr << std::endl;
	event.updateEvent(fd, POLLOUT);
}

void	cgi_server::EventHandler::handleWriteEvent(const int fd, Event& event)
{
	auto now = std::chrono::system_clock::now();
	std::time_t current_time = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&current_time), "%Y-%m-%d %H:%M:%S");

	const std::string body = std::string("<p>") + oss.str() + "</p>" + "<p>this is cgi-made response</p>";

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
