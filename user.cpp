/**
 * \file user.cpp
 * \brief Fonctions associées a l'utilisateur (User)
 * \author BOULENGER Sylvain & PIREYRE Dimitri
 * \version 1
 * \date 04 / 11 / 2012
 */
 
 #include <sstream>

#include "user.hpp"

std::string User :: generateNick ()
{
	static long seed = 0;
	
	std::ostringstream oss;		
	
	if (++seed >= 99999)
		seed = 10000;
	oss << "anon" << seed;
	return oss.str();
}

User :: User ()
{
	nickname = generateNick();
}

std::string User :: getNick () const
{
	return nickname;
}


void User :: setNick (const std::string nick)
{
	nickname = nick;
}

std::string User :: getName()
{
	return name;
}

void User :: setName (const std::string newName)
{
	name = newName;
}

std::string User :: getServer()
{
	return server;
}

void User :: setServer (const std::string newServer)
{
	server=newServer;
}
