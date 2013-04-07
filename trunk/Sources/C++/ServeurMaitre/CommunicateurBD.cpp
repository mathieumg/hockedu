//////////////////////////////////////////////////////////////////////////////
/// @file CommunicateurBD.cpp
/// @author Mathieu Parent
/// @date 2013-02-19
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "CommunicateurBD.h"
#include "ExceptionsReseau/ExceptionReseauBD.h"
#include "sha1.h"
#include <iosfwd>

SINGLETON_DECLARATION_CPP(CommunicateurBD);

bool CommunicateurBD::setupDone             = false;
std::string CommunicateurBD::mDBName        = "";
std::string CommunicateurBD::mDBPassword    = "";
std::string CommunicateurBD::mBDUser        = "";
std::string CommunicateurBD::mBDHost        = "";


////////////////////////////////////////////////////////////////////////
///
/// @fn CommunicateurBD::CommunicateurBD()
///
/// Constructeur
///
/// @return
///
////////////////////////////////////////////////////////////////////////
CommunicateurBD::CommunicateurBD()
{

    init();

}


////////////////////////////////////////////////////////////////////////
///
/// @fn CommunicateurBD::~CommunicateurBD()
///
/// Destructeur
///
/// @return
///
////////////////////////////////////////////////////////////////////////
CommunicateurBD::~CommunicateurBD()
{
	/*mConnection -> close();
	delete mConnection;*/

}



int CommunicateurBD::authenticate( const std::string& pPlayerName, const std::string& pPassword )
{
    int wUserId = -1;
	if (validateConnection())
    {
        try
        {
            unsigned char hash[20];
            char hashedPassword[41];
            sha1::calc(pPassword.c_str(),pPassword.length(), hash); // 10 is the length of the string
            sha1::toHexString(hash, hashedPassword);

            mysqlpp::Query query = mConnection.query("SELECT `id` FROM `users` WHERE `username`='"+pPlayerName+"'"+
                                                                     " AND `password`=SHA1(CONCAT('"+hashedPassword+"', SHA1( `password_salt` )))");

            if (mysqlpp::StoreQueryResult res = query.store()) {
                mysqlpp::StoreQueryResult::const_iterator it;
                for (it = res.begin(); it != res.end(); ++it) {
                    wUserId = (int)((*it)[0]); // The user's id is the first element of the returned row.
                }
            }

            if(wUserId == -1)
            {
                throw ExceptionReseau("Mot de passe invalide");
            }
        }
        catch(const mysqlpp::Exception& e)
        {
            std::cerr << "Error while reading database: " << e.what() << std::endl;
            throw ExceptionReseauBD("Erreur lors de la communication a la BD pour le authenticate");
        }

        /*mysqlpp::Connection conn = mConnection;
        mysqlpp::Query query = conn.query("select username from users");

        if (mysqlpp::StoreQueryResult res = query.store()) {
            std::cout << "We have:" << std::endl;
            mysqlpp::StoreQueryResult::const_iterator it;
            for (it = res.begin(); it != res.end(); ++it) {
                mysqlpp::Row row = *it;
                std::cout << '\t' << row[0] << std::endl;
            }
        }*/
    }
    else
    {
        throw ExceptionReseauBD("Erreur de connexion a la BD pour le authenticate");
    }
	return wUserId;
}




bool CommunicateurBD::validateConnection()
{
	/*if(mConnection -> isClosed())
	{
		throw ExceptionReseauBD("Connexion a la Base de donnee impossible");
	}*/

    if(!mConnection.connected())
    {
        // Si deconnecte, on essaie de se reconnecter
        init();
    }
    return mConnection.connected();
}



void CommunicateurBD::init()
{
    if(!CommunicateurBD::setupDone)
    {
        throw ExceptionReseauBD("Appel a init avant setup");
    }


    try 
    {
        mConnection.connect(CommunicateurBD::mDBName.c_str(), CommunicateurBD::mBDHost.c_str(), CommunicateurBD::mBDUser.c_str(), CommunicateurBD::mDBPassword.c_str());
#if !SHIPPING
        std::cout << "Connected? " << validateConnection() << std::endl;
#endif
    }
    catch(...)
    {
        std::cout << "Connexion impossible a la BD" << std::endl;
    }
}



////////////////////////////////////////////////////////////////////////
///
/// @fn CommunicateurBD::setup( const std::string& pHostname, const std::string pName, const std::string& pUser, const std::string& pPassword )
///
/// Methode pour sauvegarder les infos de connexion
///
/// @param[in]  pHostname:  Adresse de la BD (ex: hockedu.com:3306)
/// @param[in]  pName:      Nom de la base de donnee a utiliser
/// @param[in]  pUser:      Username a utiliser pour la connexion
/// @param[in]  pPassword   Password a utiliser pour la connexion
///
/// @return
///
////////////////////////////////////////////////////////////////////////
void CommunicateurBD::setup( const std::string& pHostname, const std::string pName, const std::string& pUser, const std::string& pPassword )
{
    CommunicateurBD::mBDHost        = pHostname;
    CommunicateurBD::mDBName        = pName;
    CommunicateurBD::mBDUser        = pUser;
    CommunicateurBD::mDBPassword    = pPassword;

    // Met le flag de setup seulement si tous les parametres ont au moins 1 caractere et le hostname definit le port
    CommunicateurBD::setupDone = pName.length() && pUser.length() && pPassword.length() && pHostname.find(':') != std::string::npos;

}




bool CommunicateurBD::addGameResult(const std::string& pPlayer1Name, const std::string& pPlayer2Name, int pScoreUser1, int pScoreUser2, int pTime )
{
    if (validateConnection())
    {
        std::stringstream ss;
        ss << "INSERT INTO matches (`id_user1`, `id_user2`, `score_user1`, `score_user2`, `time`) VALUES((SELECT id FROM users WHERE username = \"" + pPlayer1Name + "\"), (SELECT id FROM users WHERE username = \"" + pPlayer2Name + "\"),  "
            << pScoreUser1 << ", " << pScoreUser2 << ", " << pTime << ")";

        mysqlpp::Query query = mConnection.query(ss.str());
        try 
        {
            return query.execute();
        }
        catch(...)
        {
            return false;
        }
    }
    return false;
}










///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

