#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <cstring>

#include "user.hpp"

#ifndef _TCP_CONNECT_HPP
#define _TCP_CONNECT_HPP

class tcp_server;

class Message;


/** \class tcp_connection
* \brief Classe gerant les connexions de client 
* 
* gere la lecture et l'ecriture entre client et serveur.
*/
class tcp_connection 
	: public boost::enable_shared_from_this <tcp_connection>
{
	private:
		tcp_server* serv;
		
		User *usr;
		
		Message *msg;
		
		boost::asio::ip::tcp::socket m_socket;
		
		std::string header;
		
		boost::asio::streambuf buf;
		
		
		/**
		* \fn void tcp_connection :: handle_read 
		* 		(const boost::system::error_code& error);
		* \brief callback de l'async_write, vérifie si aucune erreur et
		* 		affiche coté serveur un message le cas échéant.
		*
		* \param error : placeholder pour d'eventuelles erreur de boost
		*/
		void handle_write (const boost::system::error_code& error);
		
		
		/**
		* \fn void tcp_connection :: handle_read 
		* 		(const boost::system::error_code& error);
		* \brief callback de l'async_read_until, vérifie si la trame lu
		* 		est trop longue (<512) est previens l'user le cas échéant
		* 		via async_write, affiche la trame recu coté serveur et 	
		* 		appel la fonction parse de message
		*
		* \param error : placeholder pour d'eventuelles erreur de boost
		* \return none
		*/
		void handle_read (const boost::system::error_code& error);
		
		
		/**
		* \fn void tcp_connection :: start_read()
		* \brief amorce la lecture (async_read_until jusqu'a "\r\n")
		* 		mise en place du hanlder (hande _read())
		*
		* \param none
		* \return none
		*/
		void start_read();
		
		
	public:
		typedef boost::shared_ptr<tcp_connection> pointer;
		
		
		/**
		 * \fn tcp_connection (boost::asio::io_service& io_service,
		 * 		tcp_server* server)
		 * \brief constructeur de tcp_connection
		 *
		 * \param io_service : reference sur io_service necessaire a 
		 * 		la gestion d'evenements.
		 * \param server : pointeur sur le serveur (tcp_server)
		 * \return Instance nouvellement allouée d'un objet de type
		 * 		tcp_connection
		 */
		tcp_connection (boost::asio::io_service& io_service, tcp_server* server);
		
		
		/**
		 * \fn ~tcp_connection ()
		 * \brief destructeur de tcp_connection
		 *
		 * \param none
		 * \return none
		 */
		~tcp_connection();
		
		
		/**
		 * \fn static pointer create
		 * 		(boost::asio::io_service& io_service,tcp_server* server)
		 * \brief constructeur de tcp_connection
		 *
		 * \param io_service : reference sur io_service necessaire 
		 * 		a la gestion d'evenements.
		 * \param server : pointeur sur le serveur (tcp_server)
		 * \return pointeur intelligent (shared_ptr) sur un instance
		 * 		nouvellement allouée d'un objet de type tcp_connection
		 */
		 static pointer create(boost::asio::io_service& ios, tcp_server*);
		 
	
		/**
		 * \fn User* getUser ()
		 * \brief getter de tcp_connection sur l'attribut usr
		 *
		 * \param none
		 * \return pointeur sur User
		 */
		User* getUser ();
		
		
		/**
		 * \fn void send (const std::string msg);
		 * \brief envoi la string msg a tcp_connection
		 *
		 * \param sring msg : une string contenant le message a envoyé
		 * \return none
		 */
		void send (const std::string);
		
		
		/**
		 * \fn boost::asio::ip::tcp::socket& socket();
		 * \brief getter sur m_socket
		 *
		 * \param none
		 * 
		 * \return boost::asio::ip::tcp::socket& : référence sur 
		 * 		m_socket.
		 */
		boost::asio::ip::tcp::socket& socket();
		
		
		/**
		* \fn void start()
		* \brief fonction d'amorçage qui demande l'ajout de this
		* 		a la map de tconnection du serveur, récupère le MOTD, 
		* 		l'envoi a son User et lance start_read().
		*
		* \param none
		* \return none
		*/
		void start();
		
		
		/**
		* \fn void reGenerateHeader()
		* \brief Fonction de re-génération du header (<nick> )
		*
		* \param none
		* \return none
		*/
		void reGenerateHeader();
};


#endif 
