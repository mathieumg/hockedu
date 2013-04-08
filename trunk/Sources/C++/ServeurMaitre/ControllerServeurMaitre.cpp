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
#include "../Reseau/RelayeurMessage.h"
#include "GameServer.h"
#include <sstream>

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
    mPaquetRunnables[EVENT]                 = PaquetRunnable::RunnableEvent;
    mPaquetRunnables[LOGIN_INFO]            = PaquetRunnable::RunnableLoginInfoMasterServer;
    // Pas de connexion automatique pour le serveur maitre
    //mPaquetRunnables[CONN_AUTOMATIQUE]    = PaquetRunnable::RunnableConnAutomatiqueServer;
    mPaquetRunnables[USER_STATUS]           = PaquetRunnable::RunnableUserStatusMasterServer;
    mPaquetRunnables[CHAT_MESSAGE]          = PaquetRunnable::RunnableChatMessageMasterServer;
    mPaquetRunnables[TEST]                  = PaquetRunnable::RunnableTest;
    mPaquetRunnables[GAME_STATUS]           = PaquetRunnable::RunnableGameStatusMasterServer;
    mPaquetRunnables[GAME_CREATION_REQUEST] = PaquetRunnable::RunnableGameCreationMasterServer;
    mPaquetRunnables[GAME_CONNECTION]       = PaquetRunnable::RunnableGameConnectionMasterServer;
    mPaquetRunnables[GAME_STATUS]           = PaquetRunnable::RunnableGameStatusMasterServer;
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
                std::string wServerIP = wAssociatedSocket->getAdresseDestination();
                GameServerManager::obtenirInstance()->addNewGameServer(i, wServerIP, wIdentificationString);

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
    case GAMES_LIST_REQUEST:
        {
            std::string wPlayerName(va_arg(pListeElems,char*));
#if !SHIPPING
            std::cout << "Games list requested" << std::endl;
#endif
            auto wGameServersContainer = GameServerManager::obtenirInstance()->getGameServersContainer();
            for(auto wGameServerIt = wGameServersContainer.begin(); wGameServerIt != wGameServersContainer.end(); ++wGameServerIt)
            {
                auto wGamesContainer = wGameServerIt->second->getGamesContainer();
                for(auto wGameIt = wGamesContainer.begin(); wGameIt != wGamesContainer.end(); ++wGameIt)
                {
                    PaquetEvent* wPaquet = (PaquetEvent*)GestionnaireReseau::obtenirInstance()->creerPaquet(EVENT);
                    wPaquet->setEventCode(GAME_ADDED);
                    std::stringstream message("");
                    auto wGame = wGameIt->second;
                    const char SEPARATOR = '|';
                    message << wGame->getServerId()         << SEPARATOR
                            << wGame->getUniqueGameId()     << SEPARATOR
                            << wGame->getGameName()         << SEPARATOR
                            << wGame->getPlayer1Name()      << SEPARATOR
                            << wGame->getPlayer2Name()      << SEPARATOR
                            << (wGame->getPassword() == "" ? "false" : "true" ) << SEPARATOR
                            << wGame->getMapName();
                    wPaquet->setMessage(message.str());
                    GestionnaireReseau::obtenirInstance()->envoyerPaquet(wPlayerName, wPaquet, TCP);
                }
            }
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
    if(pUsername == "GameServer" && pPassword == "HockeduSuperProtectedPassword")
    {
        std::string wTempUser = pUsername;
        wTempUser += GameServerManager::generateNewGameServerId();
        return wTempUser;
    }
    else
    {
        std::stringstream ss;
        ss << CommunicateurBD::obtenirInstance()->authenticate(pUsername, pPassword);
        return ss.str();
    }
}

// A implementer une fois que la structure de parties va etre faire dans le serveur maitre
void ControllerServeurMaitre::getPlayersInGame( int pGameId, std::vector<std::string>& pPlayerList )
{
    throw std::runtime_error("The method or operation is not implemented.");
}
