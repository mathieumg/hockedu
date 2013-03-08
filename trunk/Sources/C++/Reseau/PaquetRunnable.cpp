

#include "PaquetRunnable.h"
#include "GestionnaireReseau.h"
#include "Paquets/PaquetEvent.h"
#include "Paquets/PaquetLoginInfo.h"
#include "Paquets/PaquetChatMessage.h"
#include "Paquets/PaquetConnAutomatique.h"
#include "Paquets/PaquetUserStatus.h"
#include "RelayeurMessage.h"


#include <time.h>
#include <sstream>
#include <iomanip>

#include "ObjetsGlobaux/JoueurServeurs.h"
#include "Paquets/PaquetTest.h"
#include "Paquets/PaquetGameStatus.h"
#include "Paquets/PaquetMaillet.h"


// Meme pour le client et les serveurs.
// Relance l'event au gestionnaire reseau
int PaquetRunnable::RunnableEvent( Paquet* pPaquet )
{
    PaquetEvent* wPaquet = (PaquetEvent*) pPaquet;
    GestionnaireReseau::obtenirInstance()->transmitEvent(EventCodes(wPaquet->getErrorCode()), wPaquet->getMessage());

    wPaquet->removeAssociatedQuery(); // delete
    return 0;
}



// Client seulement
// Gere la reception de messages de chat
int PaquetRunnable::RunnableChatMessageClient( Paquet* pPaquet )
{
    PaquetChatMessage* wPaquet = (PaquetChatMessage*) pPaquet;

    GestionnaireReseau::obtenirInstance()->transmitEvent(CHAT_MESSAGE_RECEIVED,wPaquet->getOrigin().c_str(),wPaquet->getMessage().c_str());
    wPaquet->removeAssociatedQuery();
    return 0;
}

// Serveur jeu seulement
// Gere la reception de messages de chat
int PaquetRunnable::RunnableChatMessageServer( Paquet* pPaquet )
{
    PaquetChatMessage* wPaquet = (PaquetChatMessage*) pPaquet;

    GestionnaireReseau::obtenirInstance()->transmitEvent(CHAT_MESSAGE_RECEIVED,wPaquet->getOrigin().c_str(),wPaquet->getMessage().c_str());
    wPaquet->removeAssociatedQuery();
    return 0;
}


// Client seulement
// Doit recevoir la reponse de conn automatique du serveur
int PaquetRunnable::RunnableConnAutomatiqueClient( Paquet* pPaquet )
{
    PaquetConnAutomatique* wPaquet = (PaquetConnAutomatique*) pPaquet;
    throw std::runtime_error("Not yet implemented");
    // Implementer
    return 0;
}



// Serveur jeu seulement.
// Doit Recevoir la demande de conn automatique et repondre au client
int PaquetRunnable::RunnableConnAutomatiqueServer( Paquet* pPaquet )
{
    PaquetConnAutomatique* wPaquet = (PaquetConnAutomatique*) pPaquet;
    throw std::runtime_error("Not yet implemented");

    // Implementer
    return 0;
}

// Client seulement
// Doit gerer le status de connexion des autres utilisateurs
int PaquetRunnable::RunnableUserStatusClient( Paquet* pPaquet )
{
    PaquetUserStatus* wPaquet = (PaquetUserStatus*) pPaquet;



    switch(wPaquet->getConnectionState())
    {
    case CONNECTED:
        GestionnaireReseau::obtenirInstance()->transmitEvent(SERVER_USER_CONNECTED,wPaquet->getUserName().c_str());
        break;
    case NOT_CONNECTED:
        GestionnaireReseau::obtenirInstance()->transmitEvent(SERVER_USER_DISCONNECTED,wPaquet->getUserName().c_str());
        break;
    case CONNECTING:
        std::cout << " is reconnecting" << std::endl;
        break;
    }


    return 0;
}


// Pour serveur jeu
// Doit gerer le status de connexion des autres utilisateurs
int PaquetRunnable::RunnableUserStatusServer( Paquet* pPaquet )
{
    PaquetUserStatus* wPaquet = (PaquetUserStatus*) pPaquet;
    throw std::runtime_error("Not yet implemented");

    return 0;
}


// Global
// Pour des tests
int PaquetRunnable::RunnableTest( Paquet* pPaquet )
{
    PaquetTest* wPaquet = (PaquetTest*) pPaquet;

    std::cout << std::endl << "Message: "  << wPaquet->getMessage() << std::endl;
    std::cout << "Int: " << wPaquet->getInt() << std::endl;
    std::cout << "Float: " << wPaquet->getFloat() << std::endl;

    wPaquet->removeAssociatedQuery(); // delete
    return 0;
}


int PaquetRunnable::RunnableGameStatusClient( Paquet* pPaquet )
{
    PaquetGameStatus* wPaquet = (PaquetGameStatus*) pPaquet;
    throw std::runtime_error("Not yet implemented");

    return 0;
}

int PaquetRunnable::RunnableGameStatusServer( Paquet* pPaquet )
{
    PaquetGameStatus* wPaquet = (PaquetGameStatus*) pPaquet;
    throw std::runtime_error("Not yet implemented");

    return 0;
}



int PaquetRunnable::RunnableMailletClient( Paquet* pPaquet )
{
    PaquetMaillet* wPaquet = (PaquetMaillet*) pPaquet;
    // Modification de la pos du maillet
    throw std::runtime_error("Not yet implemented");



    return 0;
}

int PaquetRunnable::RunnableMailletServer( Paquet* pPaquet )
{
    PaquetMaillet* wPaquet = (PaquetMaillet*) pPaquet;
    // Affichage de la position du maillet

    std::cout << "Maillet:\t" << wPaquet->getPosition() << std::endl; 




    return 0;
}




