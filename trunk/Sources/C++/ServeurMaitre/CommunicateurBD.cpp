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
    try 
    {
        mConnection.connect(DB_DATABASE, DB_DBHOST, DB_USER, DB_PASSWORD);
#if !SHIPPING
        std::cout << "Connected? " << validateConnection() << std::endl;
#endif
    }
    catch(...)
    {
        throw ExceptionReseauBD("Connexion impossible a la BD");
    }
}







///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

