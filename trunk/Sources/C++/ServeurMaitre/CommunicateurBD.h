//////////////////////////////////////////////////////////////////////////////
/// @file CommunicateurBD.h
/// @author Mathieu Parent
/// @date 2013-02-19
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include "Singleton.h"

#define DBHOST "tcp://hockedu.com:3306"
#define USER "mapary"
#define PASSWORD "mz41wo13"
#define DATABASE "poly_hockedu"


///////////////////////////////////////////////////////////////////////////
/// @class CommunicateurBD
/// @brief Classe qui permet d'effectuer les requetes a la BD en lien avec le serveurMaitre
///
/// @author Mathieu Parent
/// @date 2013-02-19
///////////////////////////////////////////////////////////////////////////
class CommunicateurBD : public Singleton<CommunicateurBD>
{
	SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(CommunicateurBD);
public:
	// Constructeur
	CommunicateurBD();

	// Destructeur
	~CommunicateurBD();

	bool authenticate(const std::string& pPlayerName, const std::string& pPassword) const;


	// Methode a appeler avant chaque methode qui doit appeler la BD
	bool validerConnectiviter() const;

	void init();

private:

	

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




