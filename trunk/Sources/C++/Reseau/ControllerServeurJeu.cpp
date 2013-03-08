#include "ControllerServeurJeu.h"
#include "../Reseau/GestionnaireReseau.h"
#include "../Reseau/PaquetRunnable.h"
#include "ExceptionsReseau/ExceptionReseauSocketDeconnecte.h"
#include "PaquetRunnableServeurJeu.h"


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
    mPaquetRunnables[EVENT]                         = PaquetRunnable::RunnableEvent;
    mPaquetRunnables[USER_STATUS]                   = PaquetRunnable::RunnableUserStatusServerGame;
    mPaquetRunnables[CHAT_MESSAGE]                  = PaquetRunnable::RunnableChatMessageServerGame;
    mPaquetRunnables[GAME_STATUS]                   = PaquetRunnable::RunnableGameStatusServerGame;
    mPaquetRunnables[AUTHENTIFICATION_SERVEUR_JEU]  = PaquetRunnable::RunnableAuthentificationServeurJeuServerGame;
    mPaquetRunnables[MAILLET]                       = PaquetRunnable::RunnableMailletServer;

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
void ControllerServeurJeu::handleEvent( EventCodes pEventCode, va_list pListeElems )
{
    switch(pEventCode) {
    case USER_DISCONNECTED:
        {
            std::string& wPlayerName = va_arg(pListeElems,std::string);
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
