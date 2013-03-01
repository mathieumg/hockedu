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
#include "query.h"

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



bool CommunicateurBD::authenticate( const std::string& pPlayerName, const std::string& pPassword ) const
{

	validerConnectiviter();
    
    mysqlpp::Connection conn = mConnection;
    mysqlpp::Query query = conn.query("select username from users");
    
    if (mysqlpp::StoreQueryResult res = query.store()) {
        std::cout << "We have:" << std::endl;
        mysqlpp::StoreQueryResult::const_iterator it;
        for (it = res.begin(); it != res.end(); ++it) {
            mysqlpp::Row row = *it;
            std::cout << '\t' << row[0] << std::endl;
        }
    }

    
	return false;
}




bool CommunicateurBD::validerConnectiviter() const
{
	/*if(mConnection -> isClosed())
	{
		throw ExceptionReseauBD("Connexion a la Base de donnee impossible");
	}*/
	return false;
}



void CommunicateurBD::init()
{
    try 
    {
        mConnection.connect(DB_DATABASE, DB_DBHOST, DB_USER, DB_PASSWORD);
#if !SHIPPING
        std::cout << "Connected? " << mConnection.connected() << std::endl;
        validerConnectiviter();
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

