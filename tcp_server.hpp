#ifndef _TCP_SERVER_HPP
#define _TCP_SERVER_HPP

#include <map>
#include <string>

#include "tcp_connection.hpp"
#include "channel.hpp"
#include "tcp_message.hpp"


/** \class tcp_server
* \brief Classe gerant le serveur
* 
* fait la jonction entre les differente classe et accepte les connexions
*/
class tcp_server
{
	private:
		boost::asio::ip::tcp::acceptor m_acceptor;
		/**< socket d'ecoute*/
		
		std::string motd;
		/**< Message Of The Day*/
		
		std::map<std::string,tcp_connection::pointer>
			lUser;
		/**< map des utilisateurs <nickname, tcp_connection::pointer>*/
		
		std::map<std::string, Channel::pointer> lChan;
		/**< map des channels <nom, Channel::pointer>*/
		
		Message *msgHandler;
		
		
		/**
		 * \fn void start_accept();
		 * \brief creer une socket d'ecoute et lance l'async_accept
		 * 		dessus ainsi que la mise en place du callback
		 * 		handle_accept.
		 */
		void start_accept();
		
		
		/**
		 * \fn void handle_accept(tcp_connection::pointer new_connection, 
				const boost::system::error_code& error);
		 * \brief signale textuellement la réception d'une conexion 
		 * 		amorce la tcp_connection ( ->start() ) 
		 * 		et réarme l'async_accept 
		 * 
		 * 
		 * \param new_connection : shared_ptr sur une tcp_connection
		 * 		correspondant a la connexion entrante.
		 * \param error : placeholder boost pour d'eventuelles erreurs.
		 */
		void handle_accept(tcp_connection::pointer new_connection, 
				const boost::system::error_code& error);
						
				
	public:
		/**
		 * \fn tcp_server
		 * 		(boost::asio::io_service& io_service, int port);
		 * \brief constructeur de tcp_server
		 *
		 * 
		 * \param io_service : reference sur io_service necessaire a 
		 * 		la gestion d'evenements.
		 * \param port : numero de port sur laquel dot se connecter le
		 * 		serveur 
		 * \return Instance nouvellement allouée d'un objet de type
		 * 		tcp_server
		 */
		tcp_server(boost::asio::io_service& io_service, int port);
		
		
		/**
		 * \fn std::string tcp_server() const;
		 * \brief getter sur l'attribut MOTD
		 *
		 * 
		 * \return string contenant le MOTD du serveur.
		 */
		std::string getMOTD () const;
		
		
		/**
		 * \fn Message* getMsgHand()
		 * \brief getter sur l'attribut msgHandler
		 *
		 * 
		 * \return pointeur sur Message pointant sur le msgHandler
		 */
		Message* getMsgHandler ();
		
		
		/**
		 * \fn Channel::pointer getChan (std::string name)
		 * \brief renvoi le shared_ptr pointant sur le Channel désigné
		 * 		par name.
		 *
		 * \param name : string contenant le nom du channel désiré.
		 * \return shared_ptr sur le Channel pointant sur le Channel 
		 *	 désigné par name.
		 */
		Channel::pointer getChan (std::string);
		
		
		/**
		 * \fn 	std::list <std::string> getChanOfUser 
		 * 		(const std::string name);
		 * \brief génère une list de string contenant tous les nom
		 * 		de tous les channels rejoins par l'utilisateur désigné 
		 * 		par name
		 *
		 * \param name : string contenant le nom de l'utilisateur
		 * 		concerné
		 * \return la list de string génèrée
		 */
		std::list <std::string> getChanOfUser (const std::string);
		
		
		
		
		/**
		 * \fn void addToMap
		 *  (tcp_connection::pointer connex, const std::string nick)
		 * \brief ajoute a la map d'utilisateur la paire <nick, connex>
		 *
		 * \param connex : shared_ptr sur la connection a ajouter
		 * \param nick : string contenant le nom du User associé
		 */
		void addToMap (tcp_connection::pointer, const std::string);
		
		
		/**
		 * \fn void rmFromMap (const std::string nick)
		 * \brief retire de la map d'utilisateur l'utilisateur désigné
		 * 		par nick.
		 *
		 * \param nick : string contenant le nom de l'utilisateur
		 * 		a retirer.
		 */
		void rmFromMap (const std::string);
		
		
		/**
		 * \fn void addChanToMap (Channel::pointer connex chan, 
		 * 		const std::string name)
		 * \brief ajoute a la map de Channel la paire <name, chan>
		 *
		 * \param chan : shared_ptr sur le Channel a ajouter
		 * \param name : string contenant le nom du channel
		 */
		void addChanToMap (Channel::pointer, const std::string);
	
	
		/**
		 * \fn void rmChanFromMap (const std::string name)
		 * \brief retire de la map de Channel le Channel désigné
		 * 		par name.
		 *
		 * \param name : string contenant le nom du channel a retirer.
		 */	
		void rmChanFromMap (const std::string);
		
		
		/**
		 * \fn void addConToChan (tcp_connection::pointer con, 
		 * 		std::string chan, std::string nick)
		 * \brief ajoute un utilisateur (une connexion ici con + 
		 * 		nickname ici nick) a la map d'utilisateur du channel 
		 * 		désigné par le nom chan.
		 * 
		 * \param con : shared_ptr sur la tcp_connection a ajouter.
		 * \param chan : string contenant le nom du channel concerné.
		 * \param nick : string contenant le nom de l'utilisateur a 
		 * 		ajouter.
		 */	
		void addConToChan 
			(tcp_connection::pointer, std::string, std::string);
		
		
		
		
		/**
		 * \fn int testNick (std::string nick)
		 * \brief verifie si le nickname nick est valide soit :
		 * 		<= 9 caracteres
		 * 		sans -{(-|`.§$£¤%µ~\"^¨&;|/\\:,#+-* 
		 *
		 * \param nick : string contenant le nom d'utilisateur a tester.
		 * \return un entier variant selon les cas :
		 * 		0 le nickname est valide
		 * 		1 le nickname est deja pris
		 * 		2 le nickname contient des caracteres interdit
		 */	
		int testNick (const std::string);
		
		
		/**
		 * \fn bool testChan (std::string chan)
		 * \brief verifie si le ce nom de channel existe deja.
		 *
		 * \param chan: string contenant le nom de chan a tester.
		 * \return un boolean : false si absent de la map et donc inutilisé
		 * 	 		true si présent et donc indisponible
		 */	
		bool testChan (const std::string);
		
		
		
		
		/**
		 * \fn void repeat (const std::string msg);
		 * \brief repete msg a tous les utilisateur du serveur
		 *
		 * \param msg : string contenant le message a repeter
		 */	
		void repeat (const std::string);
        
        
        
        /**
		 * \fn  int sendToUser (std::string name, std::string msg);
		 * \brief repete msg a tous les utilisateur du serveur
		 *
		 * \param msg : string contenant le message a repeter
		 * \return int selon le resultat :
		 * 		0 si tout va bien
		 * 		1 si l'utilisateur est introuvable
		 */	
        int sendToUser (std::string, std::string);
        
        
        /**
		 * \fn  int sendToChan (std::string msg name, std::string msg, const std::string except);
		 * \brief repete msg a tous les utilisateur du chan name
		 *
		 * \param name : nom du channel a qui répéter
		 * \param msg : string contenant le message a repeter
		 * \param except : nick a qui ne pas répéter.
		 * \return int selon le resultat :
		 * 		0 si tout va bien
		 * 		1 si le channel est introuvable
		 */	   
        int sendToChan (std::string, std::string, std::string); 
        
        
        /**
		 * \fn  void changeNick (const std::string old_nick, const std::string new_nick);
		 * \brief remplace le nickname de l'utlisateur pointé par old_nick par new_nick
		 * 		et ce dans le serveur et tous les channel joints par cet utilisateur
		 *
		 * \param old_nick: ancien nickname
		 * \param new_nick : nouveau nickname
		 */	  
		void changeNick (const std::string, const std::string);


		  /**
		 * \fn std::list <std::string> getAllChanList ();
		 * \brief créer et renvoi une list contenant le nom de tous les
		 * 		channels du serveur.
		 *
		 * \return une list de string contenant les noms de tous les channels
		 */	  
		std::list <std::string> getAllChanList ();
};

#endif /* _TCP_SERVER_HPP */
