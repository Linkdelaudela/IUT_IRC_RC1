/**
 * \file tcp_connection.cpp
 * \brief Fonctions associées au connection de client (tcp_connection)
 * \author BOULENGER Sylvain & PIREYRE Dimitri
 * \version 1
 * \date 04 / 11 / 2012
 */
 
#include <iostream>
#include <stdio.h>
#include <cstring>

#include "tcp_connection.hpp"
#include "tcp_server.hpp"


typedef boost::shared_ptr<tcp_connection> pointer;


tcp_connection::pointer tcp_connection :: create(boost::asio::io_service& ios, tcp_server *serv)
{
	return tcp_connection::pointer(new tcp_connection(ios,serv));
}


boost::asio::ip::tcp::socket& tcp_connection :: socket()
{
	return m_socket;
}


void tcp_connection :: reGenerateHeader()
{
	header = "<";						/*génération du header*/
	header += usr->getNick();			/* "<nickname> "*/
	header += "> ";
}



tcp_connection ::tcp_connection(boost::asio::io_service& io_service,
		tcp_server* server)
: serv(server), m_socket(io_service)
{
	usr = new User ();
	header = "<";						/*génération du header*/
	header += usr->getNick();			/* "<nickname> "*/
	header += "> ";
}


tcp_connection::~tcp_connection()
{
}

User* tcp_connection :: getUser ()
{
	return usr;
}

void tcp_connection :: start()
{
	serv->addToMap(shared_from_this(), usr->getNick());
	/* ajout du shared_ptr de this dans la map de client du serveur */
	
	std::string m_message;
	m_message = serv->getMOTD();
	
	boost::asio::async_write(m_socket, boost::asio::buffer(m_message),
		boost::bind(&tcp_connection::handle_write, shared_from_this(),
		boost::asio::placeholders::error) );
		
	start_read();
}

void tcp_connection :: send (const std::string msg)
{
	std::cout << "SEND " << usr->getNick() << " :" << msg << std::endl<<std::endl;
	boost::asio::async_write(m_socket, boost::asio::buffer(msg),
		boost::bind(&tcp_connection::handle_write, shared_from_this(),
		boost::asio::placeholders::error) );
}


void tcp_connection :: handle_write(const boost::system::error_code& error)
{
	if (!error)
	{
		std::cout << "Ecriture OK" << std::endl;
	}
}


void tcp_connection :: start_read ()
{
	boost::asio::async_read_until(m_socket, buf,
		"\r\n", boost::bind(&tcp_connection::handle_read,
		shared_from_this(), boost::asio::placeholders::error));

}


void tcp_connection :: handle_read (const boost::system::error_code& error)
{
	if (!error)
	{
		std::string line;
		
		std::istream is(&buf);	// extraction du texte lu 
		std::getline(is, line, '\r');	// depuis le streambuf
		buf.consume(1);
		
	
		if ( line.size() > 512 ) // si msg trop long
		{
			std::string msg = "NOTICE ";
			msg += usr->getNick();
			msg += " :Message trop long ! 512 caracteres maximum\n";
		
			boost::asio::async_write(m_socket, boost::asio::buffer(msg),
			boost::bind(&tcp_connection::handle_write, shared_from_this(),
			boost::asio::placeholders::error) );
		}
		
		else
		{
			std::string to_write = header;
			to_write += line;
			to_write += "\n";
			std::cout <<to_write;
	
			serv->getMsgHandler()->parse(line, this);
			
		}
		
		start_read(); 	// réarmeement lecture
	}
}
