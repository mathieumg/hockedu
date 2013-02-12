

#include "PaquetRunnable.h"
#include "GestionnaireReseau.h"
#include "Paquets\PaquetEvent.h"
#include "Paquets\PaquetLoginInfo.h"
#include "Paquets\PaquetChatMessage.h"
#include "Paquets\PaquetConnAutomatique.h"
#include "Paquets\PaquetUserStatus.h"
#include "RelayeurMessage.h"


#include <sstream>
#include <time.h>
#include <sstream>
#include <iomanip>
#include "ObjetsGlobaux\JoueurServeurs.h"
#include "..\ServeurMaitre\FacadeServeurMaitre.h"
#include "Paquets\PaquetTest.h"


int PaquetRunnable::RunnableEvent( Paquet* pPaquet )
{
    PaquetEvent* wPaquet = (PaquetEvent*) pPaquet;
    GestionnaireReseau::obtenirInstance()->transmitEvent(wPaquet->getErrorCode(), wPaquet->getMessage());

    wPaquet->removeAssociatedQuery(); // delete
    return 0;
}


int PaquetRunnable::RunnableLoginInfoServer( Paquet* pPaquet )
{
    PaquetLoginInfo* wPaquet = (PaquetLoginInfo*) pPaquet;

    // Code pour l'authentification des users du cote du serveur maitre
#ifdef _SERVEUR_MAITRE

    // On envoie un event au gestionnaire reseau
    GestionnaireReseau::obtenirInstance()->transmitEvent(SERVER_USER_CONNECTING, wPaquet->getUsername());

    // On sauvearde le joueur
    JoueurServeurs* wJoueur = new JoueurServeurs(wPaquet->getUsername());
    FacadeServeurMaitre::obtenirInstance()->saveJoueurConnecting(wJoueur);

    // On traite la demande avec la BD
#endif


    return 0;
}


int PaquetRunnable::RunnableChatMessageServer( Paquet* pPaquet )
{
    PaquetChatMessage* wPaquet = (PaquetChatMessage*) pPaquet;


    std::ostringstream wTimeOutput;
    time_t wT = time(0);
    struct tm wTime;
    if(_localtime64_s(&wTime, &wT))
    {
        std::cout << "[00:00:00]";
    }
    else
    {
        wTimeOutput << std::setfill('0') << "["
            << std::setw(2) << wTime.tm_hour
            << std::setw(1) << ":"
            << std::setw(2) << wTime.tm_min
            << std::setw(1) << ":"
            << std::setw(2) << wTime.tm_sec
            << std::setw(1) << "]";
    }


    wTimeOutput << "[" << wPaquet->getOrigin() << "]: " << wPaquet->getMessage() << std::endl;
    std::cout << wTimeOutput.str();

    // On veut relayer le message a une personne en particulier ou a tout le monde
    // On ne call donc pas delete dessus tout suite

    if(wPaquet->IsTargetGroup())
    {
        // On envoie a tout le monde
        //std::set<std::string> wListeIgnore;
        //wListeIgnore.insert(wPaquet->getOrigin()); // Decommenter pour ne pas recevoir ses propres messages
        RelayeurMessage::obtenirInstance()->relayerPaquetGlobalement(wPaquet, NULL, TCP);
    }
    else
    {
        // On l'envoie a la personne dans groupName seulement
        RelayeurMessage::obtenirInstance()->relayerPaquet(wPaquet->getGroupName(), wPaquet, TCP);
    }

    return 0;
}


int PaquetRunnable::RunnableChatMessageClient( Paquet* pPaquet )
{
    PaquetChatMessage* wPaquet = (PaquetChatMessage*) pPaquet;

    GestionnaireReseau::obtenirInstance()->transmitEvent(CHAT_MESSAGE_RECEIVED,wPaquet->getOrigin(),wPaquet->getMessage());
    wPaquet->removeAssociatedQuery();
    return 0;
}



int PaquetRunnable::RunnableConnAutomatiqueClient( Paquet* pPaquet )
{
    PaquetConnAutomatique* wPaquet = (PaquetConnAutomatique*) pPaquet;

    // Implementer
    return 0;
}

int PaquetRunnable::RunnableConnAutomatiqueServer( Paquet* pPaquet )
{
    PaquetConnAutomatique* wPaquet = (PaquetConnAutomatique*) pPaquet;


    // Implementer
    return 0;
}


int PaquetRunnable::RunnableUserStatusClient( Paquet* pPaquet )
{
    PaquetUserStatus* wPaquet = (PaquetUserStatus*) pPaquet;



    switch(wPaquet->getConnectionState())
    {
    case CONNECTED:
        GestionnaireReseau::obtenirInstance()->transmitEvent(SERVER_USER_CONNECTED,wPaquet->getUserName());
        break;
    case NOT_CONNECTED:
        GestionnaireReseau::obtenirInstance()->transmitEvent(SERVER_USER_DISCONNECTED,wPaquet->getUserName());
        break;
    case CONNECTING:
        std::cout << " is reconnecting" << std::endl;
        break;
    }


    return 0;
}

int PaquetRunnable::RunnableUserStatusServer( Paquet* pPaquet )
{
    PaquetUserStatus* wPaquet = (PaquetUserStatus*) pPaquet;


    return 0;
}



int PaquetRunnable::RunnableTest( Paquet* pPaquet )
{
    PaquetTest* wPaquet = (PaquetTest*) pPaquet;

    std::cout << std::endl << "Message: "  << wPaquet->getMessage() << std::endl;
    std::cout << "Int: " << wPaquet->getInt() << std::endl;
    std::cout << "Float: " << wPaquet->getFloat() << std::endl;

    wPaquet->removeAssociatedQuery(); // delete
    return 0;
}






