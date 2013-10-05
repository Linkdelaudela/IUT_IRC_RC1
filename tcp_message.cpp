/**
 * \file tcp_message.cpp
 * \brief Fonctions associées au gestionnaire de message (Message)
 * \author BOULENGER Sylvain & PIREYRE Dimitri
 * \version 1
 * \date 04 / 11 / 2012
 */
 
#include <boost/tokenizer.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "tcp_message.hpp"	
#include "tcp_server.hpp"
 	
 	
Message :: Message(tcp_server* server)
:server(server)
{
	nbArg = 0;
	std::cout << "Construction d'un message" << std::endl;
}


void Message :: reset() 
{
	arguments.clear(); 
	commande.erase();
	msg.erase();
	nbArg = 0;
}

	
Message :: ~Message()
{
}


void Message :: parse(std::string trame, tcp_connection *caller)
{
	// set de cracteres de separation.
	boost::char_separator<char> sep(" ",","); 
	
	//separation de la trame reçue
	boost::tokenizer < boost::char_separator<char> > tok(trame,sep);
	boost::tokenizer < boost::char_separator<char> >::iterator
		beg=tok.begin();
	boost::tokenizer < boost::char_separator<char> >::iterator 
		end=tok.end();
	boost::tokenizer < boost::char_separator<char> >::iterator
		it=beg;
		
	int retour;
	
	commande=*beg;
	
	std::cout << "arg recu ==>" << std::endl;
	std::cout << "///___///" << std::endl;
	for(;it !=end;++it)
		++nbArg;
	std::cout << "///___///" << std::endl;
	std::cout << "Commande recue : " << commande << std::endl<< std::endl;
	
	it=beg;
	++it;
	
	
	
	/* ----------  commande NICK  ---------- */
	if( commande == "NICK")
	{
		std::cout << "New Nick : " << *it << std::endl;
		 //ajout du nickname reçu a la liste d'arguments.
		arguments.push_back(*it);
		//récuperation du nickname actuel.
		std::string old_nick = caller->getUser()->getNick();
		
		//test de la validité du nouveau nickname.
		if((retour=server->testNick(*it)) == 0)
		{
			nick(caller);	
		}
		else
		{
			std::string ret;
			
			if (retour == 1) // nouveau nickname deja pris
				ret = ":irc.localhost 433 "+old_nick+" "+*it+" :Nickname is already in use.\n";
			else // nickname contenant des caracteres interdit.
				ret = ":irc.localhost 432 "+old_nick+" "+*it+" :Erroneous Nickname.\n";
			caller->send(ret);
		}
	}
	
	
	
	/* ----------  commande PRIVMSG  ---------- */	
	else if ( commande == "PRIVMSG")
	{
		std::list<std::string> chan;
		std::list<std::string> users;
		
		// récuperation des argument
		for(;it !=end;++it)
			arguments.push_back(*it);

		for(std::list <std::string>::iterator il=arguments.begin(); il !=arguments.end();++il)
		{
			if( il->c_str()[0] == '#' || il->c_str()[0] == '&') // si ce sont des channels
				chan.push_back(*il);		// ajout a la liste des channels
			else 
				if( il->c_str()[0] == ':') // si l'argument commence par ":"
				{
					il->erase(0,1);			// on supprime le ":"
					for(std::list <std::string>::iterator il2=il; il2 !=arguments.end();++il2)
						msg +=" "+*il2;		// et on ajoute les arguments suivant dans le message
					break;
				}
				else
					users.push_back(*il);	// sinon on les ajoute a la liste des utilsiateurs
		}
		msg.erase(0,1);						// supression de l'espace en debut de message.
		privmsg(caller, chan, users, msg);
	}	
	
	
	
	/* ----------  commande USER  ---------- */
	else if ( commande == "USER")
	{
		for(;it !=end;++it) arguments.push_back(*it); // recuperation des arguments.
		user (caller);
	}
	
	
	
	/* ----------  commande QUIT  ---------- */
	else if ( commande == "QUIT")
	{
		for(;it !=end;++it) arguments.push_back(*it); // recuperation des arguments.
		quit (caller);
	}
	
	
	
	/* ----------  commande JOIN ---------- */
	else if ( commande == "JOIN")
	{
		for(;it !=end;++it) arguments.push_back(*it); // recuperation des arguments.
		join(caller);
	}
	
	
	
	/* ----------  commande TOPIC  ---------- */
	else if ( commande == "TOPIC")
	{
		for(;it !=end;++it) arguments.push_back(*it); // recuperation des arguments.
		topic (caller);
	}
	
	
	
	/* ----------  commande LIST  ---------- */
	else if ( commande == "LIST")
	{
		listChan(caller);
	}
	
	
	
	/* ----------  commande PART  ---------- */	
	else if ( commande == "PART")
	{
		for(;it !=end;++it)
			arguments.push_back(*it); // recuperation des arguments.
		part (caller);

	}
	
	else 		// si commande inconnue ou non traitée
	{
		std::string to_send = ":irc.localhost 421 "+caller->getUser()->getNick()+" "+commande+" :Unknown command.\n";
		caller->send(to_send);
	}
	
	this->reset();
}


 

void Message :: nick
    (tcp_connection *caller)
{
	
    if ( arguments.empty() )		// si aucun nickname passé en argument.
    {
		std::string ret = ":irc.localhost 431 "+caller->getUser()->getNick()+" NICK :No nickname given.\n";
		caller->send(ret);
    }

    else
    {
		std::string new_nick = *(arguments.begin());
		std::string rep = ":"+caller->getUser()->getNick()+"!@127.0.0.1 NICK :"+new_nick+"\n";
		
		server->changeNick(new_nick, caller->getUser()->getNick());		// changement du nickname coté server et coté channel
        caller->getUser()->setNick(new_nick);		// on definit ensuite le nouveau nickname
		caller->reGenerateHeader();					// regénération du header pour l'affichage coté serveur.
	
		caller->send(rep);		// envoi de la trame de reponse au client
    }
}




void Message :: privmsg
    (tcp_connection *caller,std::list<std::string> chan,
		std::list<std::string> users, std::string msg)
{	
	for(std::list<std::string>::iterator il=users.begin();
		il !=users.end(); ++il)  // parcours de la liste des utilsiateurs destinataires
	{	
		
		std::string to_send = ":"+caller->getUser()->getNick()+"!~"+caller->getUser()->getName()+"@"+caller->getUser()->getServer()+" PRIVMSG "+*il+" :"+msg+"\n";
		if ( server->sendToUser(*il, to_send) == 1 )  // si destinataire inconnu
		{
			std::string ret = ":irc.localhost 401 "+caller->getUser()->getNick()+" "+*il+" No such nick/channel.\n";
			caller->send(to_send);  // envoi de la trame de reponse au client
		}
	}

	for(std::list<std::string>::iterator il2=chan.begin();
		il2 !=chan.end(); ++il2)  // parcours de ma liste des channels destinataires
	{	
		std::string to_send = ":"+caller->getUser()->getNick()+"!~"+caller->getUser()->getName()+"@"+caller->getUser()->getServer()+" PRIVMSG "+*il2+" :"+msg+"\n" ;
		if ( server->sendToChan(*il2, to_send, caller->getUser()->getNick()) == 1 ) // si destinataire inconnu
		{
			std::string ret = ":irc.localhost 401 "+caller->getUser()->getNick()+" "+*il2+" No such nick/channel.\n";
			caller->send(ret);  // envoi de la trame de reponse au client
		}
	}
}




void Message :: join (tcp_connection *caller)
{
	std::string ret;
	
	for(std::list<std::string>::iterator il=arguments.begin(); il !=arguments.end(); ++il)
	{	// parcours de la liste d'arguments contenant les channels a joindre
		std::string chanName = *il;
		
		if( chanName.c_str()[0] == '#' || chanName.c_str()[0] == '&')   
		{		// test de l'existances et de la validité des channels
			if (!server->testChan(*il))
			{	// si inexistant et correct alors creation du channel
				Channel::pointer chan = Channel::create
					(*il, server);
				
				chan->addToServ();	// ajout du channel au serveur
				
				chan->addToMapChan(caller->shared_from_this(),
						"@"+caller->getUser()->getNick());		// ajout du caller au channel
						
				ret = ":"+caller->getUser()->getNick()+"!@127.0.0.1 JOIN :"+chanName+"\n"; // on previens que le caller a rejoins le channel
				chan->repeat(ret,"");
				chan->listUser(caller->shared_from_this());		// on liste les utilisateurs du channel
			} 	
											
			else
			{
				// on ajoute le caller au channel
				server->addConToChan (caller->shared_from_this(), 
						chanName,caller->getUser()->getNick());
						
				ret = ":"+caller->getUser()->getNick()+"!@127.0.0.1 JOIN :"+chanName+"\n";
				server->sendToChan(chanName, ret,"");  				// on previens que le caller a rejoins le channel
				server->getChan(chanName)->listUser(caller->shared_from_this()); 		// on liste les utilisateurs du channel
			}
		}
	}
}



void Message :: user (tcp_connection *caller)
{
	std::list<std::string>::iterator il=arguments.begin();
	
	// recupération des nom d'utilsateurs et de l'adresse du server du client
	caller->getUser()->setName(*il);
	++il; ++il;
	caller->getUser()->setServer(*il);		
}

void Message :: quit (tcp_connection *caller)
{
	std::cout << "Commande QUIT" << std::endl;
	std::string nick = caller->getUser()->getNick();
	std::list <std::string> listChan= server->getChanOfUser(nick);
	std::string message;
	
	for (std::list<std::string> :: iterator it1 = arguments.begin(); it1 != arguments.end(); ++it1)
		message += " "+*it1; // composition du message de depart de l'utilisateurs.
	
	message.erase (0,1); // suppression de l'espace de debut.
	
	std::string to_send = ":"+nick+"!~"+caller->getUser()->getName()+"@"+caller->getUser()->getServer()+" QUIT :"+message+"\n";
	server->rmFromMap(nick);	// suppression de l'utilisateur du serveur
	
	for (std::list <std::string>::iterator it=listChan.begin();	
		it != listChan.end(); ++it) 
	// suppression de l'utilisateur de chaque channels qu'il avait joint.
	{
		server->getChan(*it)->rmFromMapChan(nick);
		server->getChan(*it)->repeat(to_send,"");
	}
}




void Message :: listChan (tcp_connection *caller)
{
	//recuperation de la liste des channels du serveur
	std::list <std::string> list_chan = server->getAllChanList(); 
	std::list <std::string> :: iterator it;
	std::string to_send;
	
	for ( it = list_chan.begin(); it != list_chan.end(); ++it) 
	// envoi d'une trame de description pour chaque channel.
	{
		to_send = ":irc.localhost 322 "+caller->getUser()->getNick()+" "+*it+" "+boost::lexical_cast<std::string>(server->getChan(*it)->getNbUser())+" :"+server->getChan(*it)->getTopic()+".\n";
		caller->send(to_send);
	}
	
	// trame de fin de liste
	to_send = ":irc.localhost 323 "+caller->getUser()->getNick()+" :End of LIST.\n";
	caller->send(to_send);
}



void Message :: topic (tcp_connection *caller)
{
	
	std::list <std::string> :: iterator it = arguments.begin();
	std::string channel = *it;
	std::string topic;
	
	++it;
	
	if (server->testChan(channel))
	// si channel inexistant.
	{
		std::string send = ":irc.localhost 403"+caller->getUser()->getNick()+" "+channel+" :No such channel.\n";
		caller->send(send);
	}
	
	if (nbArg < 3)
	// si affichage du topic pour le chan cible
	{
		if ( (topic = server->getChan(channel)->getTopic()).empty() )
		// si aucun topic défini pour le chan cible
		{
			std::string send = ":irc.localhost 331 "+caller->getUser()->getNick()+" "+channel+" :No topic is set.\n";
			caller->send(send);
		}

		else
		{
			std::string send = ":"+channel+"!~"+caller->getUser()->getName()+"@"+caller->getUser()->getServer()+" TOPIC "+channel+" :"+topic+"\n";
			server->getChan(channel)->repeat(send,"");
		}	
	}
	
	else
	// si définition d'un nouveau topic pour le chan cible
	{
		for (; it != arguments.end(); ++it)
			topic += *it;

		server->getChan(channel)->setTopic(topic);
		std::string send = ":"+channel+"!~"+caller->getUser()->getName()+"@"+caller->getUser()->getServer()+" TOPIC "+channel+" :"+topic+"\n";
		server->getChan(channel)->repeat(send,"");
	}
}


void Message :: part(tcp_connection *caller)
{
	std::list<std::string> chan;
	std::string msg;

	for(std::list <std::string>::iterator il=arguments.begin(); il !=arguments.end();++il)
	{  // parcours de la liste des arguments.
		if( il->c_str()[0] == '#' || il->c_str()[0] == '&')		// si channel ajout a la lsite chan
			chan.push_back(*il);
		else 
			if( il->c_str()[0] == ':')		// sinon message
			{
				il->erase(0,1); //supression du ":"
				 // construction du message avec tous les arguments restant.
				for(std::list <std::string>::iterator il2=il; il2 !=arguments.end();++il2)
					msg +=" "+*il2;
				break;
			}
	}
	msg.erase(0,1);  // supression de l'espace de debut.
	
	std::list <std::string> :: iterator il2;
	for (il2 = chan.begin(); il2 != chan.end(); il2++)
	//parcours de la liste de channel 
	{
		if(server->testChan(*il2))		// si channel valide
		{
			if ( server->getChan(*il2)->isConnected(caller->getUser()->getNick()) )
			// si utilisateur effectivement connecté a ce channel
			{
				std::string to_send = ":"+caller->getUser()->getNick()+"!~"+caller->getUser()->getName()+"@"+caller->getUser()->getServer()+" PART "+*il2+" :"+msg+"\n";
				server->getChan(*il2)->rmFromMapChan(caller->getUser()->getNick()); // deconnexion du channel
				
				server->getChan(*il2)->repeat(to_send,"");
			}
			
			else
			// si channel non joint par le caller
			{
				std::string to_send = ":irc.localhost 442 "+caller->getUser()->getNick()+" "+*il2+" :You're not on that channel.\n";
				caller->send(to_send);
			}
		}

		else
		// si channel inexistant
		{
			std::string to_send	= ":irc.localhost 403 "+caller->getUser()->getNick()+" "+*il2+" :No such channel.\n";
		}
	}
}
