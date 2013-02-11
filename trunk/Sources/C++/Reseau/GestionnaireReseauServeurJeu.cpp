///////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireReseauServeurJeu.cpp
/// @author Mathieu Parent
/// @date 2013-02-10
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "GestionnaireReseauServeurJeu.h"
#include "..\Reseau\PaquetHandlers\PacketHandlerChatMessage.h"
#include "..\Reseau\UsinePaquets\UsinePaquetChatMessage.h"
#include "Reseau\GestionnaireReseau.h"
#include "Paquets\PaquetChatUserStatus.h"
#include "PaquetHandlers\PacketHandlerChatUserStatus.h"
#include "UsinePaquets\UsinePaquetChatUserStatus.h"
#include "RelayeurMessage.h"

// Initialisations automatiques
SINGLETON_DECLARATION_CPP(GestionnaireReseauServeurJeu);

////////////////////////////////////////////////////////////////////////
///
/// @fn  GestionnaireReseauServeurJeu::GestionnaireReseauServeurJeu(  )
///
/// Constructeur, appelle le constructeur de GestionnaireReseau
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GestionnaireReseauServeurJeu::GestionnaireReseauServeurJeu()
{
    // Initialisation du GestionnaireReseau
    GestionnaireReseau::setNetworkMode(SERVER);
	GestionnaireReseau::obtenirInstance()->setSocketConnectionStateCallback(SocketStateCallback);
	//GestionnaireReseau::setObserverSocketServer(new SocketObserverServerChat());
    GestionnaireReseau* wGestionnaireReseau = GestionnaireReseau::obtenirInstance();
    wGestionnaireReseau->init();

    // On doit ajouter une nouvelle operation reseau pour que le systeme le connaisse (1 par type de paquet), trompez-vous pas pcq sa va chier en ti pepere!
	wGestionnaireReseau->ajouterOperationReseau("ChatMessage", new PacketHandlerChatMessage, new UsinePaquetChatMessage);
	wGestionnaireReseau->ajouterOperationReseau("ChatUserStatus", new PacketHandlerChatUserStatus, new UsinePaquetChatUserStatus);


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
GestionnaireReseauServeurJeu::~GestionnaireReseauServeurJeu()
{
    GestionnaireReseau::libererInstance();
}

void GestionnaireReseauServeurJeu::SocketStateCallback( const ConnectionStateEvent& pEvent )
{
	// Le socket a change de state, on envoie une notification aux autres

	const ConnectionState& wConnState = pEvent.mState;
	const std::string& wPlayerName = pEvent.mPlayerName;

	PaquetChatUserStatus* wPaquet = (PaquetChatUserStatus*) GestionnaireReseau::obtenirInstance()->creerPaquet("ChatUserStatus");
	
	if(wConnState == CONNECTED) // Si le socket vient de se connecter, on lui envoie la liste de tous les players connectes
	{
		std::set<std::string> wListe = GestionnaireReseau::obtenirInstance()->getPlayerNameList(TCP);
		SPSocket wSocketVientConnecter = GestionnaireReseau::obtenirInstance()->getSocket(wPlayerName, TCP);
		for(std::set<std::string>::iterator it = wListe.begin(); it!=wListe.end(); ++it)
		{
			if(wPlayerName != (*it))
			{
				// On l'envoie a playerName
				PaquetChatUserStatus* wPaquet = (PaquetChatUserStatus*) GestionnaireReseau::obtenirInstance()->creerPaquet("ChatUserStatus");

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