#include "ControllerServeurJeu.h"
#include "..\Reseau\GestionnaireReseau.h"
#include "..\Reseau\PaquetRunnable.h"


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
/// Reception d'un �v�nement provenant du r�seau
///
/// @param[in] int pEventCode
/// @param[in] va_list pListeElems
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ControllerServeurJeu::handleEvent( int pEventCode, va_list pListeElems )
{
//     switch(pEventCode) {
//     case USER_DISCONNECTED:
//         {
//             std::string wPlayerName = va_arg(pListeElems,std::string);
//             if(wPlayerName.length() != 0 )
//             {
//                 // Handle deconnection
//             }
//             break;
//         }
//     case SERVER_USER_CONNECTING:
//         {
//             std::cout << "Event: Tentative de connexion de " << va_arg(pListeElems,std::string) << std::endl;
//             break;
//         }
//     default:
//         std::cout << "EventCode: " << pEventCode << std::endl;
//         break;
//     };
}