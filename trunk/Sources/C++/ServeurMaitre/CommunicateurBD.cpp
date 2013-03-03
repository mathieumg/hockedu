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
        }
        catch(const mysqlpp::Exception& e)
        {
            std::cerr << "Error while reading database: " << e.what() << std::endl;
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
	return wUserId;
}




bool CommunicateurBD::validateConnection() const
{
	/*if(mConnection -> isClosed())
	{
		throw ExceptionReseauBD("Connexion a la Base de donnee impossible");
	}*/
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
        throw ExceptionReseauBD("Connexion impossible a la BD");
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










///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

