#include "Server.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <poll.h>
#include <stdexcept>
#include <stdio.h>
#include <cstring>
#include <iostream>

cgi_server::Server::Server(const std::string& address, const unsigned int port)
{
	initListenSocket(address, port);
}

cgi_server::Server::~Server() {}

void	cgi_server::Server::initListenSocket(const std::string& address, const unsigned int port)
{
	const int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_socket == -1)
		throw std::runtime_error(std::string("socket: ") + strerror(errno));
	
	struct sockaddr_in	svaddr;
	std::memset(&svaddr, 0, sizeof(struct sockaddr_in));
	if (inet_pton(AF_INET, address.c_str(), &svaddr) == -1)
		throw std::runtime_error(std::string("inet_pton failed for address ") + address + strerror(errno));
	svaddr.sin_family = AF_INET;
	svaddr.sin_port = htons(port);

	if (bind(listen_socket, (sockaddr*)&svaddr, sizeof(struct sockaddr_in)) == -1)
		throw std::runtime_error(std::string("bind: ") + strerror(errno));

	if (listen(listen_socket, SOMAXCONN) == -1)
		throw std::runtime_error(std::string("listen: ") + strerror(errno));
	
	event_.setListenSocket(listen_socket);
	event_.addEvent(listen_socket, POLLIN);
	std::cerr << "server is running on " << address << ":" << port << std::endl;
}

void	cgi_server::Server::run()
{
	event_.eventloop();
}
