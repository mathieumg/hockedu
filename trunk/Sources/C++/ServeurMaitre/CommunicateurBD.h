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

	int authenticate(const std::string& pPlayerName, const std::string& pPassword);

	void init();

    // Methode pour sauvegarder les infos de connexion
    static void setup(const std::string& pHostname, const std::string pName, const std::string& pUser, const std::string& pPassword);

private:
    // Methode a appeler avant chaque methode qui doit appeler la BD
    bool validateConnection() const;

	mysqlpp::Connection mConnection;

    static std::string mBDHost;
    static std::string mBDUser;
    static std::string mDBPassword;
    static std::string mDBName;

    static bool setupDone;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




