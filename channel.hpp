#include <list>
#include <cstring>
#include <map>
#include <boost/enable_shared_from_this.hpp>

#ifndef _CHAN_HPP
#define _CHAN_HPP

#include "tcp_connection.hpp"

class tcp_server;

/** 
* \class Channel
* \brief Classe gerant les channels
* 
* fait les traitement sur les channels
*/
class Channel
	: public boost::enable_shared_from_this<Channel>
{
	private :
		tcp_server *server;
		
		std::map <std::string, tcp_connection::pointer> lUserChan;

		std::string topic;
		
		std::string chanName;
		
		std::string mode;
		
		
	public :
		
		typedef boost::shared_ptr<Channel> pointer;
		
		
		/**
		 * \fn Channel
		 * 		(const std::string name, tcp_server* server)
		 * \brief constructeur de Channel
		 *
		 * \param name : nom du channel a creer
		 * \param server : pointeur sur le serveur (tcp_server)
		 * \return Instance nouvellement allouée d'un objet de type
		 * 		Channel
		 */
		Channel (const std::string, tcp_server*);
		
		std::string getName ();
		 
		 /**
		 * \fn static pointer create
		 * 		(const std::string name, boost::asio::io_service&
		 * 		 io_service,tcp_server* server)
		 * \brief constructeur de Channel
		 *
		 * \param name : nom du channel a creer
		 * \param server : pointeur sur le serveur (tcp_server)
		 * \return pointeur intelligent (shared_ptr) sur un instance
		 * 		nouvellement allouée d'un objet de type Channel
		 */
		static pointer create(const std::string, tcp_server *);
		
		
		
		/**
		 * \fn int getNbUser ();
		 * \brief renvoi le nombre d'utilisateurs présent sur le channel
		 *
		 * \return entier contenant le nombre d'utilsiateurs
		 */
		int getNbUser();
		
		
		
		
		/**
		 * \fn void setTopic (std::string newTopic);
		 * \brief setter sur l'attribut topic
		 *
		 * \param newName : string contenant le nouveau topic
		 */
		void setTopic(std::string);
		
		
		/**
		 * \fn std::string getTopic ();
		 * \brief getter sur l'attribut topic
		 *
		 * \return string contenant le topic
		 */
		std::string getTopic ();
		
		
		
		/**
		 * \fn void addToMapChan (tcp_connection::pointer connex, 
				const std::string nick)
		 * \brief ajout d'une tcp_connection et de son nickname a la map
		 * 		d'utilisateur.
		 *
		 * \param connex : shared_ptr sur tcp_connection, connexion a 
		 * 		ajouter a la map de client.
		 * \param nick : string désignant le nickname de la connexion a
		 * 		ajouter.
		 */
		void addToMapChan (tcp_connection::pointer, const std::string);
		
		
		/**
		 * \fn void rmFromMapChan (const std::string nick)
		 * \brief retirer une tcp_connection de la map d'utilisateurs
		 * 		via son nickname
		 *
		 * \param nick : string désignant le nickname de la connexion a
		 * 		retirer.
		 */
		void rmFromMapChan (const std::string);
		
		
		/**
		 * \fn void listUser (tcp_connection::pointer whoAsk)
		 * \brief lister les utilisaterus du channel a l'utilisateur
		 * 		pointé par whoAsk.
		 *
		 * \param whoAsk : shared_ptr sur tcp_connection désignant l'utilisateur 
		 * 		faisant la requete.
		 */
		void listUser (tcp_connection::pointer);
		
		
		/**
		 * \fn void repeat (const std::string msg, const std::string except);
		 * \brief répète le message msg a tous les utilisateurs du channel,
		 * 		toutes les tcp_connection de la map.
		 *
		 * \param msg : string contenant le message a transmettre
		 * \param except : nickname a qui ne pas répéter
		 */
		void repeat (const std::string, const std::string) ;
		
		
		/**
		 * \fn void getUserList ();
		 * \brief génére une list avec le nickname de tous ls utilisateurs de la map 
		 *
		 * \return list de string contenant tous les nick des utilisateurs connectes
		 */
		std::list <std::string>  getUserList ();


		/**
		 * \fn void addToServ ();
		 * \brief ajoute this a la map de chan du serveur.
		 */
		void addToServ();
		
		
		/**
		 * \fn void changeNick (const std::string old_nick, const std::string new_nick);
		 * \brief change le nickname de l'utilsateur désigné par old_nick en new_nick.
		 *
		 * \param old_nick : ancien nickname de l'utilisateur
		 * \param new_nick : nouveau nickname de l'utilisateur
		 */
		void changeNick (const std::string, const std::string);
		
		
		/**
		 * \fn bool isConnected (const std::string nick)
		 * \brief vérifie si le client désigné par nick est sur ce channel.
		 *
		 * \param nick : nickname de l'utilisateur a vérifié.
		 * \return boolean true si présent false sinon.
		 */
		bool isConnected (const std::string);
};

#endif /* _CHAN_HPP */ 
