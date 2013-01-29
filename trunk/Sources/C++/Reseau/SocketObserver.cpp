///////////////////////////////////////////////////////////////////////////////
/// @file SocketObserver.cpp
/// @author Mathieu Parent
/// @date 2013-01-28
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "SocketObserver.h"
#include "Paquets\PaquetChatUserStatus.h"
#include "Reseau\GestionnaireReseau.h"
#include "RelayeurMessage.h"
#include <set>


////////////////////////////////////////////////////////////////////////
///
/// @fn  SocketObserver::SocketObserver(  )
///
/// Constructeur
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
SocketObserverServerChat::SocketObserverServerChat()
{
    

}


////////////////////////////////////////////////////////////////////////
///
/// @fn  SocketObserver::~SocketObserver(  )
///
/// Destructeur 
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
SocketObserverServerChat::~SocketObserverServerChat()
{
    
}





void SocketObserverServerChat::updateObserver( Socket* pSubject )
{
	// Le socket a change de state, on envoie une notification aux autres
	ConnectionState wConnState = pSubject->getConnectionState();

	PaquetChatUserStatus* wPaquet = (PaquetChatUserStatus*) GestionnaireReseau::obtenirInstance()->creerPaquet("ChatUserStatus");
	std::string wPlayerName = GestionnaireReseau::obtenirInstance()->getPlayerName(pSubject);
	if("" == wPlayerName)
	{
		delete wPaquet;
	}
	else
	{
		wPaquet->setUserName(wPlayerName);
		wPaquet->setConnectionState(wConnState);

		//std::set<std::string> wTemp;
		RelayeurMessage::obtenirInstance()->relayerPaquetGlobalement(wPaquet);
	}
	




}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////