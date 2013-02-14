

#include "PaquetRunnable.h"
#include "GestionnaireReseau.h"
#include "Paquets\PaquetEvent.h"
#include "Paquets\PaquetLoginInfo.h"
#include "Paquets\PaquetChatMessage.h"
#include "Paquets\PaquetConnAutomatique.h"
#include "Paquets\PaquetUserStatus.h"
#include "RelayeurMessage.h"


#include <time.h>
#include <sstream>
#include <iomanip>
#include "ObjetsGlobaux\JoueurServeurs.h"
#include "Paquets\PaquetTest.h"


// Meme pour le client et les serveurs. 
// Relance l'event au gestionnaire reseau
int PaquetRunnable::RunnableEvent( Paquet* pPaquet )
{
    PaquetEvent* wPaquet = (PaquetEvent*) pPaquet;
    GestionnaireReseau::obtenirInstance()->transmitEvent(wPaquet->getErrorCode(), wPaquet->getMessage());

    wPaquet->removeAssociatedQuery(); // delete
    return 0;
}



// Client seulement 
// Gere la reception de messages de chat
int PaquetRunnable::RunnableChatMessageClient( Paquet* pPaquet )
{
    PaquetChatMessage* wPaquet = (PaquetChatMessage*) pPaquet;

    GestionnaireReseau::obtenirInstance()->transmitEvent(CHAT_MESSAGE_RECEIVED,wPaquet->getOrigin(),wPaquet->getMessage());
    wPaquet->removeAssociatedQuery();
    return 0;
}

// Serveur jeu seulement 
// Gere la reception de messages de chat
int PaquetRunnable::RunnableChatMessageServer( Paquet* pPaquet )
{
    PaquetChatMessage* wPaquet = (PaquetChatMessage*) pPaquet;

    GestionnaireReseau::obtenirInstance()->transmitEvent(CHAT_MESSAGE_RECEIVED,wPaquet->getOrigin(),wPaquet->getMessage());
    wPaquet->removeAssociatedQuery();
    return 0;
}


// Client seulement
// Doit recevoir la reponse de conn automatique du serveur
int PaquetRunnable::RunnableConnAutomatiqueClient( Paquet* pPaquet )
{
    PaquetConnAutomatique* wPaquet = (PaquetConnAutomatique*) pPaquet;

    // Implementer
    return 0;
}



// Serveur jeu seulement. 
// Doit Recevoir la demande de conn automatique et repondre au client
int PaquetRunnable::RunnableConnAutomatiqueServer( Paquet* pPaquet )
{
    PaquetConnAutomatique* wPaquet = (PaquetConnAutomatique*) pPaquet;


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


// Pour serveur jeu
// Doit gerer le status de connexion des autres utilisateurs
int PaquetRunnable::RunnableUserStatusServer( Paquet* pPaquet )
{
    PaquetUserStatus* wPaquet = (PaquetUserStatus*) pPaquet;


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



