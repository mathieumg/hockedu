#include "ControllerCSharp.h"
#include "GestionnaireReseau.h"
#include "PaquetRunnable.h"
#include "PaquetRunnableClientLourd.h"
#ifdef LINUX
#include <stdarg.h>
#endif
#include "GameManager.h"
#include "Partie.h"
#include "PartieSyncer.h"


int CallbackSetPatieSyncerClientLourd(int pGameId, GameStatus)
{
    Partie* wGame = GameManager::obtenirInstance()->getGame(pGameId);
    if(wGame)
    {
        wGame->getPartieSyncer()->addDestinationIdentifier("GameServer");
    }
    return 0;
}



////////////////////////////////////////////////////////////////////////
///
/// @fn  ControllerCSharp::ControllerCSharp()
///
/// Constructeur
///
///
/// @return
///
////////////////////////////////////////////////////////////////////////
ControllerCSharp::ControllerCSharp():mEventReceivedCallback(NULL),mMessageReceivedCallBack(NULL)
{
    // Ajouter tous les Runnables dependant de ce qui est handled selon le type de controlleur
    mPaquetRunnables[EVENT]                 = PaquetRunnable::RunnableEvent;
    mPaquetRunnables[CONN_AUTOMATIQUE]      = PaquetRunnable::RunnableConnAutomatiqueClient;
    mPaquetRunnables[USER_STATUS]           = PaquetRunnable::RunnableUserStatusClient;
    mPaquetRunnables[CHAT_MESSAGE]          = PaquetRunnable::RunnableChatMessageClient;
    mPaquetRunnables[TEST]                  = PaquetRunnable::RunnableTest;
    mPaquetRunnables[MAILLET]               = PaquetRunnable::RunnableMailletClient;
    mPaquetRunnables[RONDELLE]              = PaquetRunnable::RunnableRondelleClient;
    mPaquetRunnables[GAME_CREATION_REQUEST] = PaquetRunnable::RunnableGameCreationClient;
    mPaquetRunnables[GAME_CONNECTION]       = PaquetRunnable::RunnableGameConnectionClient;
    mPaquetRunnables[GAME_EVENT]            = PaquetRunnable::RunnableGameEventClient;



    for(EventCodes e = EventCodes(SERVER_EVENT_BEGIN+1); e<SERVER_EVENT_END; e = EventCodes(e+1))
    {
        mEventTypeHandlers[e] = ControllerCSharp::HandleEvent;
    }

    mEventTypeHandlers[CHAT_MESSAGE_RECEIVED] = ControllerCSharp::HandleMessage;

    for(EventCodes e = EventCodes(MODEL_EVENT_BEGIN+1); e<MODEL_EVENT_END; e = EventCodes(e+1))
    {
        mEventTypeHandlers[e] = ControllerCSharp::HandleEvent;
    }


    GameManager::obtenirInstance()->addGameUpdateCallback(CallbackSetPatieSyncerClientLourd);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void ControllerCSharp::handleEvent( int pEventCode, va_list pListeElems )
///
/// Reception d'un événement provenant du réseau
///
/// @param[in] int pEventCode
/// @param[in] va_list pListeElems
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ControllerCSharp::handleEvent( EventCodes pEventCode, va_list pListeElems )
{
    int wEventHandled = false;
    auto it = mEventTypeHandlers.find(EventCodes(pEventCode));
    if(it != mEventTypeHandlers.end())
    {
        wEventHandled = (it->second)(this,pEventCode,pListeElems);
    }
//     if(!wEventHandled)
//     {
//         std::cerr << "UnHandled event\n";
//     }
}


void ControllerCSharp::handleDisconnectDetection( SPSocket pSocket )
{
    GestionnaireReseau::obtenirInstance()->demarrerConnectionThread(pSocket);
    GestionnaireReseau::obtenirInstance()->supprimerEcouteSocket(pSocket);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ControllerCSharp::HandleEvent( EventCodes, va_list pListeElems )
///
/// /*Description*/
///
/// @param[in] EventCodes
/// @param[in] va_list pListeElems
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
int ControllerCSharp::HandleEvent(ControllerCSharp* pContext, EventCodes pEventCode, va_list pListeElems )
{
    EventReceivedCallBack c = pContext->mEventReceivedCallback;
    if(c)
    {
        std::string message;
        if(pEventCode == SERVER_USER_DISCONNECTED || pEventCode == SERVER_USER_CONNECTED)
        {
            message = va_arg(pListeElems,char*);
        }
        return c(pEventCode,(char*)message.c_str());
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn int ControllerCSharp::HandleMessage( void*, EventCodes, va_list pListeElems )
///
/// /*Description*/
///
/// @param[in] void *
/// @param[in] EventCodes
/// @param[in] va_list pListeElems
///
/// @return int
///
////////////////////////////////////////////////////////////////////////
int ControllerCSharp::HandleMessage( ControllerCSharp* pContext, EventCodes pEventCode, va_list pListeElems )
{
    MessageReceivedCallBack c = pContext->mMessageReceivedCallBack;
    if(c)
    {
        std::string username    = va_arg(pListeElems,char*);
        std::string message     = va_arg(pListeElems,char*);
        return c((char*)username.c_str(),(char*)message.c_str());
    }
    return 0;
}



void ControllerCSharp::getPlayersInGame( int pGameId, std::vector<const std::string>& pPlayerList )
{
    pPlayerList.push_back("GameServer");
}
