/*
*
* user.hpp
*
*/
#ifndef _USER_HPP
#define _USER_HPP

#include <cstring>

/** \class User
* \brief Classe gerant les infos utilisateur
*/
class User
{
	private :
		std::string nickname;
		std::string name;
		std::string server;

		
	public :
		 /**
		 * \fn User ()
		 * \brief constructeur de User
		 *
		 * \return Instance nouvellement allouée d'un objet de type
		 * 		Client
		 */
		User ();
		
		
		
		  /**
		 * \fn std::string getNick () const
		 * \brief getter sur l'attribut nickname
		 *
		 * \return une string contenant le nickname.
		 */	 
		std::string getNick() const;


		 /**
		 * \fn  void setNick (const std::string nick)
		 * \brief setter sur l'attribut nickname
		 * 		définit nick comme nouveau nickname
		 *
		 * \param une string contenant le nouveau nickname.
		 */	 
		void setNick (const std::string);
		
		
		
		/**
		 * \fn  static std::string generateNick ()
		 * \brief génère un nickname de compris entre anon12345 et 
		 * 		anon99999, a chaque génération il est incrémenter.
		 *
		 * \return une string contenant le nouveau nickname généré.
		 */	 
		static std::string generateNick ();
		
		std::string getName();

		void setName (const std::string);

		std::string getServer();
		
		void setServer (const std::string);
};

#endif /* _USER_HPP */
