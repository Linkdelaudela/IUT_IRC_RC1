#ifndef _TCP_MESSAGE_HPP
#define _TCP_MESSAGE_HPP

#include <list>
#include <string>
#include "tcp_connection.hpp"

#include "channel.hpp"

class tcp_server;

class Message
{
	private:
		std::list<std::string>	arguments; 
		
		std::string commande;
		
		std::string	msg; 
		
		tcp_server* server;
		
		int nbArg;
		
		
		  /**
		 * \fn void notice (tcp_connection *caller, std::string msg);
		 * \brief fonction corresponant a la commande NOTICE 
		 * 		notifie le message msg a l'utilisateur pointé par caller
		 *
		 * \param caller : pointeur sur tcp_connection pointant sur 
		 * 		l'utilisateur viser par la methode
		 * \param msg : string contenant le message a notfier
		 */	  
		void notice (tcp_connection *, std::string);
		
		
		/**
		 * \fn void nick (tcp_connection *caller);
		 * \brief fonction corresponant a la commande NICK
		 * 		modifie le nickname de l'utilisateur pointé par 
		 * 		caller avec celui se trouvant dans l'attribut arguments.
		 *
		 * \param caller : pointeur sur tcp_connection pointant sur 
		 * 		l'utilisateur appelant la methode
		 */	  
        void nick (tcp_connection *);
        
        
        /**
		 * \fn void privmsg 
		 * 		(tcp_connection *caller,std::list<std::string> chan,
				std::list<std::string> users, std::string msg)
		 * \brief fonction corresponant a la commande PRIVMSG
		 * 		envoi le message msg au destinataires se trouvant
		 * 		dans les listes chan et users.
		 * 		
		 * 		Si les 2 listes sont vide le msg est répété a tout le
		 * 		serveur
		 *
		 * \param caller : pointeur sur tcp_connection pointant sur 
		 * 		l'utilisateur appelant la methode
		 * \param msg : string contenant le message a envoyer
		 * \param chan : list de string contenant les noms des channels
		 * 		destinataires.
		 * \param users : list de string contenant les nickname des 
		 * 		utilisateurs destinataires.
		 */	 
		void privmsg (tcp_connection *, std::list<std::string>,
			std::list<std::string>, std::string);

	
	public:

		Message(tcp_server*);	

		~Message();


		
		/**
		 * \fn void reset();
		 * \brief remet a zero les champs commande et arguments
		 */	  
		void reset();
		
		
		
		/**
		 * \fn void parse(std::string trame, tcp_connection *caller)
		 * \brief remplis les champs commande et arguments en parsant
		 * 		la chaine de caractere reçue par le serveur.
		 * 		appel les fonction appropriée ensuite.
		 * 
		 * \param trame : string contenant la trame recu par le serveur
		 * \param caller : pointeur sur tcp_connection pointant sur 
		 * 		l'utilisateur appelant la methode
		 */
		void parse(std::string, tcp_connection *);


		/**
		 * \fn 	void join (tcp_connection *caller)
		 * \brief fonction traitant la commande JOIN.
		 * 
		 * \param caller : pointeur sur l'utlisateur appelant la fonction.
		 */
		void join (tcp_connection*);
	
	
		/**
		 * \fn 	void join (tcp_connection *caller)
		 * \brief fonction traitant la commande JOIN.
		 * 
		 * \param caller : pointeur sur l'utlisateur appelant la fonction.
		 */
		void user (tcp_connection*);
		
		
		/**
		 * \fn 	void quit (tcp_connection *caller)
		 * \brief fonction traitant la commande QUIT
		 * 
		 * \param caller : pointeur sur l'utlisateur appelant la fonction.
		 */
		void quit (tcp_connection*);
		
		
		/**
		 * \fn 	void listChan(tcp_connection *caller)
		 * \brief fonction traitant la commande LIST
		 * 
		 * \param caller : pointeur sur l'utlisateur appelant la fonction.
		 */
		void listChan (tcp_connection *);
		
		
		/**
		 * \fn 	void topic (tcp_connection *caller)
		 * \brief fonction traitant la commande TOPIC
		 * 
		 * \param caller : pointeur sur l'utlisateur appelant la fonction.
		 */
		void topic (tcp_connection *);
		
		
		/**
		 * \fn 	void part (tcp_connection *caller)
		 * \brief fonction traitant la commande PART
		 * 
		 * \param caller : pointeur sur l'utlisateur appelant la fonction.
		 */
		void part(tcp_connection *);
};


#endif /* _TCP_MESSAGE_HPP */
