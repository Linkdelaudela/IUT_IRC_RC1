/**
 * \file channel.cpp
 * \brief Fonctions associées aux channels (Channel)
 * \author BOULENGER Sylvain & PIREYRE Dimitri
 * \version 1
 * \date 04 / 11 / 2012
 */

#include <list>

#include "channel.hpp"
#include "tcp_server.hpp"

Channel::pointer Channel :: create(const std::string name,
		tcp_server *serv)
{
	return Channel::pointer(new Channel(name, serv));
}


Channel :: Channel 
	(const std::string name, tcp_server *serv)
{
	chanName = name;
	server = serv;
	
	std::string to_send = "Le channel "+chanName+" a ete cree.";
	std::cout << to_send <<std::endl;
}

void Channel :: addToServ()
{
	server->addChanToMap(shared_from_this(), chanName);
}

std::string Channel ::  getTopic ()
{
	return topic;
}

void Channel :: setTopic (std::string newTopic)
{
	topic=newTopic;
}

void Channel :: addToMapChan (tcp_connection::pointer connex, 
	const std::string nick)
{
	lUserChan.insert(std::pair <std::string, tcp_connection::pointer>
		(nick, connex));
	std::cout << nick << " a rejoins le channel " << chanName
		<< std::endl;
}

		
void Channel :: rmFromMapChan (const std::string nick)
{
	lUserChan.erase (nick);
	std::cout << nick << " a quitter le channel " << chanName
		<< std::endl;
		
	if ( lUserChan.empty() )
	{
		server->rmChanFromMap(chanName);
	}
}

void Channel :: listUser (tcp_connection::pointer whoAsk)
{
	std::map<std::string, tcp_connection::pointer>::
		iterator it;
	std::string to_send;
	
	for (it = lUserChan.begin(); it != lUserChan.end(); ++it)
	{
		/*:irc.localhost 353 kamnikazB = #lol :@kamnikazB */
		to_send = ":irc.localhost 353 "+whoAsk->getUser()->getNick()+" = "+chanName+" :"+it->first+"\n";
		whoAsk->send(to_send);
	}
	to_send = ":irc.localhost 366 "+whoAsk->getUser()->getNick()+" "+chanName+" :End of NAMES list.\n";
	whoAsk->send(to_send);
}

void Channel :: repeat (const std::string msg, const std::string except)
{
	std::map<std::string, tcp_connection::pointer>::
		iterator it;

	for (it = lUserChan.begin(); it != lUserChan.end(); ++it)
		if (it->first != except && it->first != "@"+except)
			it->second->send(msg);
}

std::list <std::string> Channel :: getUserList ()
{
	std::list <std::string> lOfUser;
	
	std::map<std::string, tcp_connection::pointer>::
		iterator it;

	for (it = lUserChan.begin(); it != lUserChan.end(); ++it)
		lOfUser.push_back (it->first);
	
	return lOfUser;
}

std::string Channel :: getName ()
{
	return chanName;
}


void Channel :: changeNick (const std::string new_nick, const std::string old_nick)
{
	std::map<std::string, tcp_connection::pointer>::iterator it;
	
	it = lUserChan.find(old_nick);
	
	if (it != lUserChan.end())
	{
		lUserChan.insert(std::pair <std::string, tcp_connection::pointer>
			(new_nick, it->second));
			
		lUserChan.erase(it);
	}
}

int Channel :: getNbUser()
{
	return (int)lUserChan.size();
}

bool Channel :: isConnected (const std::string nick)
{
	std::map<std::string, tcp_connection::pointer> :: iterator it;
	
	it=lUserChan.find(nick);
	
	if (it == lUserChan.end())
		return false;
		
	return true;
}
