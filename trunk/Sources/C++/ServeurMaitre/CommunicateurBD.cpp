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
#include "ExceptionsReseau\ExceptionReseauBD.h"

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
	/*

	Statement* wStatement = mConnection -> createStatement();
	ResultSet* rs = wStatement -> executeQuery ("select * from users");



	// retrieve the row count in the result set 
	std::cout << "\nRetrieved " << rs -> rowsCount() << " row(s)." << std::endl;

	std::cout << "\nUsernames" << std::endl;
	std::cout << "--------" << std::endl;

	// fetch the data : retrieve all the rows in the result set 
	while (rs->next()) {
		std::cout << rs -> getString("username") << std::endl;
	} // while

	std::cout << std::endl;

	*/
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

	

}







///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

