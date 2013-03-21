#include "ControllerServeurMaitre.h"
#include "../Reseau/GestionnaireReseau.h"
#include "../Reseau/PaquetRunnable.h"
#include "PaquetRunnableServeurMaitre.h"
#include "../Reseau/ExceptionsReseau/ExceptionReseauSocketDeconnecte.h"
#include "CommunicateurBD.h"
#include "ExceptionsReseau/ExceptionReseauBD.h"
#include "../Reseau/Paquets/PaquetEvent.h"
#include "../Reseau/ObjetsGlobaux/PartieServeurs.h"
#include "GameServerManager.h"

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
    mPaquetRunnables[LOGIN_INFO] = PaquetRunnable::RunnableLoginInfoMasterServer;
    // Pas de connexion automatique pour le serveur maitre
    //mPaquetRunnables[CONN_AUTOMATIQUE] = PaquetRunnable::RunnableConnAutomatiqueServer;
    mPaquetRunnables[USER_STATUS] = PaquetRunnable::RunnableUserStatusMasterServer;
    mPaquetRunnables[CHAT_MESSAGE] = PaquetRunnable::RunnableChatMessageMasterServer;
    mPaquetRunnables[TEST] = PaquetRunnable::RunnableTest;
    mPaquetRunnables[GAME_STATUS] = PaquetRunnable::RunnableGameStatusMasterServer;
    mPaquetRunnables[GAME_REGISTRATION] = PaquetRunnable::RunnableGameRegistrationMasterServer;
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
void ControllerServeurMaitre::handleEvent( EventCodes pEventCode, va_list pListeElems )
{
    switch(pEventCode) {
    case USER_DISCONNECTED:
        {
            std::string wPlayerName(va_arg(pListeElems,char*));
            if(wPlayerName.length() != 0 )
            {
                // Handle deconnection (throw une exception qui sera catch dans le thread de reception et qui fera la deconnexion)
                throw ExceptionReseauSocketDeconnecte("Message de deconnection recu par le client " + wPlayerName);
            }
            break;
        }
    case GAME_SERVER_AUTHENTICATION_REQUEST:
        {
#if !SHIPPING
            std::cout << "Game server authentication request" << std::endl;
#endif
            for(auto i = GameServerManager::getAddedGameServersAmount() + 1; i <= GameServerManager::getLatestGameServerId(); ++i)
            {
                std::string wIdentificationString("GameServer");
                wIdentificationString += i;
                SPSocket wAssociatedSocket = GestionnaireReseau::obtenirInstance()->getSocket(wIdentificationString, TCP);

                GameServerManager::obtenirInstance()->addNewGameServer(i, wAssociatedSocket->getAdresseDestination());

                PaquetEvent* wReplyPacket = (PaquetEvent*)GestionnaireReseau::obtenirInstance()->creerPaquet(EVENT);
                wReplyPacket->setEventCode(GAME_SERVER_AUTHENTICATION_REPLY);
                std::stringstream message;
                message << i;
                wReplyPacket->setMessage(message.str());
#if !SHIPPING
                std::cout << "Replying to server id " << i << ". The server's IP is: " << wAssociatedSocket->getAdresseDestination() << std::endl;
#endif
                GestionnaireReseau::obtenirInstance()->envoyerPaquet(wAssociatedSocket, wReplyPacket);
            }
            break;
        }
    case SERVER_USER_CONNECTING:
        {
            std::cout << "Event: Tentative de connexion de " << va_arg(pListeElems,char*) << std::endl;
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


// Retourne l'identifiant du serveur
std::string ControllerServeurMaitre::authenticate( const std::string& pUsername, const std::string& pPassword )
{
    try
    {
        if(pUsername == "GameServer" && pPassword == "HockeduSuperProtectedPassword")
        {
            std::string wTempUser = pUsername;
            wTempUser += GameServerManager::generateNewGameServerId();
            return wTempUser;
        }
        else if (CommunicateurBD::obtenirInstance()->authenticate(pUsername, pPassword) != -1)
        {
            return pUsername;
        }
        else
        {
            return "";
        }
    }
    catch(ExceptionReseauBD&)
    {
        return "";
    }
}

// A implementer une fois que la structure de parties va etre faire dans le serveur maitre
void ControllerServeurMaitre::getPlayersInGame( int pGameId, std::vector<const std::string*>& pPlayerList )
{
    throw std::runtime_error("The method or operation is not implemented.");
}
