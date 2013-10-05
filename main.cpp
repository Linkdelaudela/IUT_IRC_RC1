
#include "tcp_server.hpp"


int main(int argc, char** argv)
{	
	
	boost::asio::io_service io_service;
	
	/* Création d'un serveur */
	new tcp_server (io_service, 41750);
	io_service.run();
			
	return 0;
}
	
 
