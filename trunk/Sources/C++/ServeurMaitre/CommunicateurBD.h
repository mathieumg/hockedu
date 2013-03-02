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
#include "mysql++.h"

#define DB_DBHOST "hockedu.com:3306" // hockedu.com:3306
#define DB_USER "mapary"
#define DB_PASSWORD "mz41wo13"
#define DB_DATABASE "poly_hockedu"


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

	mysqlpp::Connection mConnection;

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




