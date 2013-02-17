///////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireReseauServeurMaitre.cpp
/// @author Mathieu Parent
/// @date 2013-02-09
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "GestionnaireReseauServeurMaitre.h"
#include "GestionnaireReseau.h"
#include "Paquets\PaquetUserStatus.h"
#include "PaquetHandlers\PacketHandlerUserStatus.h"
#include "UsinePaquets\UsinePaquetUserStatus.h"
#include "RelayeurMessage.h"
#include "PaquetHandlers\PacketHandlerChatMessage.h"
#include "UsinePaquets\UsinePaquetChatMessage.h"
#include "UsinePaquets\UsinePaquetGameStatus.h"
#include "PaquetHandlers\PacketHandlerGameStatus.h"

// Initialisations automatiques
SINGLETON_DECLARATION_CPP(GestionnaireReseauServeurMaitre);

////////////////////////////////////////////////////////////////////////
///
/// @fn  GestionnaireReseauServeur::GestionnaireReseauServeur(  )
///
/// Constructeur, appelle le constructeur de GestionnaireReseau
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GestionnaireReseauServeurMaitre::GestionnaireReseauServeurMaitre()
{
    // Initialisation du GestionnaireReseau
	GestionnaireReseau::obtenirInstance()->setSocketConnectionStateCallback(SocketStateCallback);
	//GestionnaireReseau::setObserverSocketServer(new SocketObserverServerChat());
    GestionnaireReseau* wGestionnaireReseau = GestionnaireReseau::obtenirInstance();
    wGestionnaireReseau->initServer();

    // On doit ajouter une nouvelle operation reseau pour que le systeme le connaisse (1 par type de paquet), trompez-vous pas pcq sa va chier en ti pepere!
	wGestionnaireReseau->ajouterOperationReseau(CHAT_MESSAGE, new PacketHandlerChatMessage, new UsinePaquetChatMessage);
	wGestionnaireReseau->ajouterOperationReseau(USER_STATUS, new PacketHandlerUserStatus, new UsinePaquetUserStatus);
    wGestionnaireReseau->ajouterOperationReseau(GAME_STATUS, new PacketHandlerGameStatus, new UsinePaquetGameStatus);


}


////////////////////////////////////////////////////////////////////////
///
/// @fn  GestionnaireReseauServeur::~GestionnaireReseauServeur(  )
///
/// Destructeur 
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GestionnaireReseauServeurMaitre::~GestionnaireReseauServeurMaitre()
{
    GestionnaireReseau::libererInstance();
}

void GestionnaireReseauServeurMaitre::SocketStateCallback( const ConnectionStateEvent& pEvent )
{
	// Le socket a change de state, on envoie une notification aux autres

	const ConnectionState& wConnState = pEvent.mState;
	const std::string& wPlayerName = pEvent.mPlayerName;

	PaquetUserStatus* wPaquet = (PaquetUserStatus*) GestionnaireReseau::obtenirInstance()->creerPaquet(USER_STATUS);
	
	if(wConnState == CONNECTED) // Si le socket vient de se connecter, on lui envoie la liste de tous les players connectes
	{
		std::set<std::string> wListe = GestionnaireReseau::obtenirInstance()->getPlayerNameList(TCP);
		SPSocket wSocketVientConnecter = GestionnaireReseau::obtenirInstance()->getSocket(wPlayerName, TCP);
		for(std::set<std::string>::iterator it = wListe.begin(); it!=wListe.end(); ++it)
		{
			if(wPlayerName != (*it))
			{
				// On l'envoie a playerName
				PaquetUserStatus* wPaquet = (PaquetUserStatus*) GestionnaireReseau::obtenirInstance()->creerPaquet(USER_STATUS);

				wPaquet->setUserName((*it));
				wPaquet->setConnectionState(wSocketVientConnecter->getConnectionState());

				RelayeurMessage::obtenirInstance()->relayerPaquet(wPlayerName, wPaquet, TCP);

			}
		}
	}
	if(wPlayerName.size() == 0)
	{
        wPaquet->removeAssociatedQuery(); // delete
	}
	else
	{
		wPaquet->setUserName(wPlayerName);
		wPaquet->setConnectionState(wConnState);

		RelayeurMessage::obtenirInstance()->relayerPaquetGlobalement(wPaquet);
	}
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////