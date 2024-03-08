#include <iostream>
#include <exception>

#include "Server.hpp"

int	main()
{
	try
	{
		cgi_server::Server	server("127.0.0.1", 9000);
		server.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return 1;
}