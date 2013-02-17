#include "ControllerServeurMaitre.h"
#include "..\Reseau\GestionnaireReseau.h"
#include "..\Reseau\PaquetRunnable.h"
#include "PaquetRunnableServeurMaitre.h"
#include "..\Reseau\ExceptionsReseau\ExceptionReseauSocketDeconnecte.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn  ControllerServeurMaitre::ControllerServeurMaitre()
///
/// Constructeur
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
ControllerServeurMaitre::ControllerServeurMaitre()
{
    // Ajouter tous les Runnables dependant de ce qui est handled selon le type de controlleur
    mPaquetRunnables[EVENT] = PaquetRunnable::RunnableEvent;
    mPaquetRunnables[LOGIN_INFO] = PaquetRunnable::RunnableLoginInfoServerMaster;
    // Pas de connexion automatique pour le serveur maitre
    //mPaquetRunnables[CONN_AUTOMATIQUE] = PaquetRunnable::RunnableConnAutomatiqueServer;
    mPaquetRunnables[USER_STATUS] = PaquetRunnable::RunnableUserStatusServerMaster;
    mPaquetRunnables[CHAT_MESSAGE] = PaquetRunnable::RunnableChatMessageServerMaster;
    mPaquetRunnables[TEST] = PaquetRunnable::RunnableTest;
    mPaquetRunnables[GAME_STATUS] = PaquetRunnable::RunnableGameStatusServerMaster;

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void ControllerServeurMaitre::handleEvent( int pEventCode, va_list pListeElems )
///
/// Reception d'un événement provenant du réseau
///
/// @param[in] int pEventCode
/// @param[in] va_list pListeElems
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ControllerServeurMaitre::handleEvent( int pEventCode, va_list pListeElems )
{
    switch(pEventCode) {
    case USER_DISCONNECTED:
        {
            std::string wPlayerName = va_arg(pListeElems,std::string);
            if(wPlayerName.length() != 0 )
            {
                // Handle deconnection (throw une exception qui sera catch dans le thread de reception et qui fera la deconnexion)
                throw ExceptionReseauSocketDeconnecte("Message de deconnection recu par le client " + wPlayerName);
            }
            break;
        }
    case SERVER_USER_CONNECTING:
        {
            std::cout << "Event: Tentative de connexion de " << va_arg(pListeElems,std::string) << std::endl;
            break;
        }
    default:
        std::cout << "EventCode: " << pEventCode << std::endl;
        break;
    };
}




void ControllerServeurMaitre::handleDisconnectDetection(SPSocket pSocket)
{
    GestionnaireReseau::obtenirInstance()->removeSocket(pSocket);
}
