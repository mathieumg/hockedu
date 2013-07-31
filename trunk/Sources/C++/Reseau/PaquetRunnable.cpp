

#include "PaquetRunnable.h"
#include "GestionnaireReseau.h"
#include "Paquets/PaquetEvent.h"
#include "Paquets/PaquetLoginInfo.h"
#include "Paquets/PaquetUserStatus.h"
#include "RelayeurMessage.h"


#include <time.h>
#include <sstream>
#include <iomanip>

#include "ObjetsGlobaux/JoueurServeurs.h"
#include "Paquets/PaquetGameStatus.h"
#include "Paquets/PaquetMaillet.h"
#include <iostream>
#include "Paquets/PaquetGameConnection.h"
#include "Paquets/PaquetBonus.h"

// Meme pour le client et les serveurs.
// Relance l'event au gestionnaire reseau
int PaquetRunnable::RunnableEvent( Paquet* pPaquet )
{
    PaquetEvent* wPaquet = (PaquetEvent*) pPaquet;
    GestionnaireReseau::obtenirInstance()->transmitEvent(EventCodes(wPaquet->getEventCode()), wPaquet->getMessage().c_str());

    wPaquet->removeAssociatedQuery(); // delete
    return 0;
}



// Client seulement
// Gere la reception de messages de chat
int PaquetRunnable::RunnableChatMessageClient( Paquet* pPaquet )
{
    PacketDataChatMessage* wData = (PacketDataChatMessage*) pPaquet->getData();
    GestionnaireReseau::obtenirInstance()->transmitEvent(CHAT_MESSAGE_RECEIVED,wData->mOrigin.c_str(),wData->mMessage.c_str(), wData->mGroupName.c_str());
    
    pPaquet->removeAssociatedQuery();
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






