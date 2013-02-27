#include "ControllerServeurJeu.h"
#include "../Reseau/GestionnaireReseau.h"
#include "../Reseau/PaquetRunnable.h"
#include "ExceptionsReseau/ExceptionReseauSocketDeconnecte.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn  ControllerServeurJeu::ControllerServeurJeu()
///
/// Constructeur
///
///
/// @return
///
////////////////////////////////////////////////////////////////////////
ControllerServeurJeu::ControllerServeurJeu()
{
    // Ajouter tous les Runnables dependant de ce qui est handled selon le type de controlleur
    mPaquetRunnables[EVENT] = PaquetRunnable::RunnableEvent;
    //mPaquetRunnables[LOGIN_INFO] = PaquetRunnable::RunnableLoginInfoServer;
    mPaquetRunnables[CONN_AUTOMATIQUE] = PaquetRunnable::RunnableConnAutomatiqueServer;
    mPaquetRunnables[USER_STATUS] = PaquetRunnable::RunnableUserStatusServer;
    mPaquetRunnables[CHAT_MESSAGE] = PaquetRunnable::RunnableChatMessageServer;
    mPaquetRunnables[TEST] = PaquetRunnable::RunnableTest;
    mPaquetRunnables[GAME_STATUS] = PaquetRunnable::RunnableGameStatusServer;


}


////////////////////////////////////////////////////////////////////////
///
/// @fn void ControllerServeurJeu::handleEvent( int pEventCode, va_list pListeElems )
///
/// Reception d'un événement provenant du réseau
///
/// @param[in] int pEventCode
/// @param[in] va_list pListeElems
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ControllerServeurJeu::handleEvent( int pEventCode, va_list pListeElems )
{
    switch(pEventCode) {
    case USER_DISCONNECTED:
        {
            std::string wPlayerName = va_arg(pListeElems,std::string);
            if(wPlayerName.length() != 0 )
            {
                // Handle deconnection
                throw ExceptionReseauSocketDeconnecte("Socket deconnecte");
            }
            break;
        }
    default:
        std::cout << "EventCode: " << pEventCode << std::endl;
        break;
    };
}

void ControllerServeurJeu::handleDisconnectDetection( SPSocket pSocket )
{
    GestionnaireReseau::obtenirInstance()->removeSocket(pSocket);
}
