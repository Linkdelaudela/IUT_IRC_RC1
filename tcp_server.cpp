/**
 * \file tcp_server.cpp
 * \brief Fonctions associées au serveur IRC (tcp_server)
 * \author BOULENGER Sylvain & PIREYRE Dimitri
 * \version 1
 * \date 04 / 11 / 2012
 */

#include <list>
#include <algorithm>

#include <boost/tokenizer.hpp>

#include "tcp_server.hpp"

tcp_server::tcp_server (boost::asio::io_service& io_service,int port)
	: m_acceptor(io_service, boost::asio::ip::tcp::endpoint
		(boost::asio::ip::tcp::v4(), port))
{
	msgHandler = new Message (this);		// creation du msgHandler
	
	motd = "Bonjour et bienvenue sur le serveur IRC de Dimitri "
			"et Sylvain !\n"
			"Vous allez vous voir attribuer un pseudo aleatoire que "
			"vous pourrez changer grace a la commande NICK.\n";
			
	start_accept();		// lancement du serveur
	
}


void tcp_server :: start_accept()
{
	tcp_connection::pointer new_connection;
	
	// creation de la socket d'ecoute.
	new_connection = tcp_connection::create(m_acceptor.io_service(),
						this);
						
	//lancement de l'async_accept sur la socket d'ecoute et mise en
	//place du callback handle_accept
	m_acceptor.async_accept(new_connection->socket(),
		boost::bind(&tcp_server::handle_accept, this, new_connection,
		boost::asio::placeholders::error));
	
}

	
void tcp_server ::  handle_accept (tcp_connection::pointer
	new_connection, const boost::system::error_code& error)
{
	if (!error)
	{
		std::cout << "Reçu un client!" << std::endl;
		new_connection->start();
		// amorce de la nouvelle connexion
		
		// réarmement de l'async_accept
		start_accept();
	}
}


Message* tcp_server :: getMsgHandler ()
{
	return msgHandler;;
}


std::string tcp_server :: getMOTD () const
{
	return motd;
}


void tcp_server :: addToMap (tcp_connection::pointer connex,
	const std::string nick)
{
	lUser.insert (std::pair <std::string, tcp_connection::pointer> 
		(nick, connex));
}


void tcp_server :: repeat (const std::string msg)
{
	std::map<std::string, tcp_connection::pointer>
		:: iterator it;

	for (it = lUser.begin(); it != lUser.end(); ++it)
		it->second->send(msg);
}

void tcp_server :: rmFromMap (const std::string nick)
{
	lUser.erase (nick);
	std::cout << "Utilisateur " << nick << " supprime" << std::endl;
}


int tcp_server :: testNick (const std::string nick)
{
    if (nick.c_str()[0] == '&' || nick.c_str()[0] == '#' || nick.c_str()[0] == '+')
		return 2;
	else
	{
		std::map<std::string,tcp_connection::pointer>
			:: iterator it;
			
		it=lUser.find(nick);
		
		if (it == lUser.end())
			return 0;
		else
			return 1;
	}
		
	
}

void tcp_server :: addChanToMap (Channel::pointer chan,
	const std::string name)
{
	lChan.insert (std::pair <std::string, Channel::pointer> 
		(name, chan));
		
	
	std::cout << "Contenu map :" << std::endl;
	for (std::map<std::string, Channel::pointer> :: iterator it = lChan.begin(); it != lChan.end(); ++it)
	{
		std::cout << it->first << std::endl;
		std::cout << it->second.use_count() << std::endl;
	}
}
		
void tcp_server :: rmChanFromMap (const std::string name)
{
	lChan.erase (name);
	std::cout << "channel" << name << " supprime" << std::endl;
}

bool tcp_server :: testChan (const std::string name)
{
	std::map<std::string, Channel::pointer> :: iterator it;
	std::cout << '[' << name << ']' << std::endl;
	it=lChan.find(name);
	
	if (it == lChan.end())
		return false;
		
	return true;
}

Channel::pointer tcp_server :: getChan (std::string name)
{
	std::map<std::string, Channel::pointer> :: iterator it;
	it=lChan.find(name);
		
	return  it->second;
}

std::list <std::string> tcp_server :: getChanOfUser 
	(const std::string name)
{
	std::list <std::string> listChan;
	std::list <std::string> listUser;
	
	std::list <std::string>::iterator ilist;
	std::map<std::string, Channel::pointer>::iterator imap;

	// on cherche dans la liste des utilisateurs de chque Channel
	// un nickname recherché
	for (imap = lChan.begin(); imap != lChan.end(); ++imap)
	{
		listUser = imap->second->getUserList();
		
		for(ilist = listUser.begin(); ilist !=listUser.end();++ilist)
		{
			if(*ilist == name) //si le nick correspond, on ajoute le
				listChan.push_back (imap->first);// chan a la liste.
		}
	}
	return listChan;
}

int tcp_server :: sendToUser (std::string name, std::string msg)
{
	std::map<std::string,tcp_connection::pointer>::
		iterator imap;
	imap = lUser.find (name);
	
	if (imap == lUser.end())
		return 1;
	
	imap->second->send(msg);
	
	return 0;
}

int tcp_server :: sendToChan (std::string name, std::string msg, std::string except)
{
	std::map<std::string, Channel::pointer>::iterator imap;
	imap = lChan.find (name);
	
	if (imap == lChan.end())
		return 1;
	
	imap->second->repeat(msg, except);
	
	return 0;
}

void tcp_server :: addConToChan
	(tcp_connection::pointer con, std::string chan, std::string nick)
{
	std::map<std::string, Channel::pointer>::iterator imap;
	imap = lChan.find (chan);
	
	imap->second->addToMapChan(con, nick);
}

void tcp_server :: changeNick (const std::string new_nick, const std::string old_nick)
{
	std::map<std::string, tcp_connection::pointer>::iterator imap;
	imap = lUser.find (old_nick);
	
	if (imap != lUser.end())
	{
		lUser.insert (std::pair <std::string, tcp_connection::pointer> 
		(new_nick,imap->second));
		lUser.erase(imap);
				
		std::list <std::string> chanList = getChanOfUser(old_nick);
		
		for (std::list <std::string> :: iterator ilist = chanList.begin(); ilist != chanList.end(); ++ilist)
		{
			Channel::pointer to_call = lChan.find (*ilist)->second;
			to_call->changeNick(old_nick, new_nick);
		}
	}
}

std::list <std::string> tcp_server :: getAllChanList ()
{
	std::map<std::string, Channel::pointer>::iterator imap;
	std::list <std::string> list;
	
	for (imap = lChan.begin(); imap != lChan.end(); ++imap)
		list.push_back(imap->first);
		
	return list;
}
