

#include "PaquetRunnableServeurMaitre.h"
#include "../ServeurMaitre/FacadeServeurMaitre.h"
#include "../Reseau/Paquets/PaquetLoginInfo.h"
#include "../Reseau/GestionnaireReseau.h"
#include "../Reseau/ObjetsGlobaux/JoueurServeurs.h"
#include "../Reseau/RelayeurMessage.h"
#include "../Reseau/Paquets/PaquetUserStatus.h"

#ifdef LINUX
#define _LARGE_TIME_API
#endif

#include <time.h>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "../Reseau/Paquets/PaquetGameStatus.h"
#include "../Reseau/Paquets/PaquetGameCreation.h"
#include "GameServerManager.h"
#include "GameServer.h"
#include "../reseau/Paquets/PaquetGameConnection.h"
#include <set>

/// ***** PAR CONVENTION, METTRE Master A LA FIN DU NOM DES DELEGATES


int PaquetRunnable::RunnableLoginInfoMasterServer( Paquet* pPaquet )
{
    PaquetLoginInfo* wPaquet = (PaquetLoginInfo*) pPaquet;

    // Code pour l'authentification des users du cote du serveur maitre


    // On envoie un event au gestionnaire reseau
    GestionnaireReseau::obtenirInstance()->transmitEvent(SERVER_USER_CONNECTING, wPaquet->getUsername().c_str());

    // On sauvearde le joueur
    JoueurServeurs* wJoueur = new JoueurServeurs(wPaquet->getUsername());
    FacadeServeurMaitre::obtenirInstance()->savePlayerConnecting(wJoueur);

    // On traite la demande avec la BD



    return 0;
}





int PaquetRunnable::RunnableChatMessageMasterServer( Paquet* pPaquet )
{
    PacketDataChatMessage* wData = (PacketDataChatMessage*) pPaquet->getData();

    std::ostringstream wTimeOutput;
    time_t wT = time(0);
    struct tm* wTime = NULL;
#ifdef WINDOWS
    struct tm wTimeWin;
    if(_localtime64_s(&wTimeWin, &wT))
    {
#elif defined(LINUX)
    time(&wT);
    wTime = localtime(&wT);
    if(wTime == NULL)
    {
#endif

		// If time == NULL
        std::cout << "[00:00:00]";
    }
    else
    {
#ifdef WINDOWS
		wTime = &wTimeWin;
#endif
        wTimeOutput << std::setfill('0') << "["
            << std::setw(2) << wTime->tm_hour
            << std::setw(1) << ":"
            << std::setw(2) << wTime->tm_min
            << std::setw(1) << ":"
            << std::setw(2) << wTime->tm_sec
            << std::setw(1) << "]";
    }


    wTimeOutput << "[" << wData->mOrigin << "]: " << wData->mMessage << std::endl;
    std::cout << wTimeOutput.str();

    // On veut relayer le message a une personne en particulier ou a tout le monde
    // On ne call donc pas delete dessus tout suite
    if(wData->mIsTargetGroup)
    {
        // On l'envoie a la personne dans groupName seulement
        RelayeurMessage::obtenirInstance()->relayerPaquet(wData->mGroupName, pPaquet, TCP);
    }
    else
    {
        // On envoie a tout le monde
        std::set<std::string> wListeIgnore;
        wListeIgnore.insert("GameServer");
        RelayeurMessage::obtenirInstance()->relayerPaquetGlobalement(pPaquet, &wListeIgnore, TCP);
    }

    return 0;
}



int PaquetRunnable::RunnableUserStatusMasterServer( Paquet* pPaquet )
{
    PaquetUserStatus* wPaquet = (PaquetUserStatus*) pPaquet;
    throw std::runtime_error("Not yet implemented");

    return 0;
}




int PaquetRunnable::RunnableGameCreationMasterServer( Paquet* pPaquet )
{
    PaquetGameCreation* wPaquet = (PaquetGameCreation*) pPaquet;

    const int wGameId = wPaquet->getGameId();
    unsigned int wServerId = wPaquet->getServerId();
    if ( wGameId == -1 )
    {
        if( wServerId == 0)
        {
            wServerId = GameServerManager::obtenirInstance()->selectGameServer();
            if(wServerId == -1)
            {
                wPaquet->setGameId(-1);
                GestionnaireReseau::obtenirInstance()->envoyerPaquet(wPaquet->getUsername(), wPaquet, TCP);
                return 0;
            }
            wPaquet->setServerId(wServerId);
        }

#if !SHIPPING
        std::cout << "Forwarding packet to GameServer " << wServerId << std::endl;
#endif

        std::string wServerIdentifier = GameServerManager::obtenirInstance()->getGameServer(wServerId)->getServerIdentifier();
        GestionnaireReseau::obtenirInstance()->envoyerPaquet(wServerIdentifier, wPaquet, TCP);
    }
    else
    {
#if !SHIPPING
        std::cout << "Creating game id " << wGameId << " on server " << wServerId << std::endl;
#endif
        GameServerManager::obtenirInstance()->getGameServer(wServerId)->addGame(wGameId, wPaquet->getGameName(), wPaquet->getMapName(), wPaquet->getUsername(), wPaquet->getPassword());
        GameServer* wServer = GameServerManager::obtenirInstance()->getGameServer(wServerId);
        std::string wServerIP = wServer->getServerIP();
#if !SHIPPING
        std::cout << "Setting GameServer IP to " << wServerIP << " in packet reply" << std::endl;
#endif
        wPaquet->setServerIP(wServerIP);
#if !SHIPPING
        std::cout << "Forwarding reply packet to user " << wServerId << std::endl;
#endif

        // Sends reply to game creator
        GestionnaireReseau::obtenirInstance()->envoyerPaquet(wPaquet->getUsername(), wPaquet, TCP);
    }
    return 0;
}




int PaquetRunnable::RunnableGameConnectionMasterServer( Paquet* pPaquet )
{
    PaquetGameConnection* wPaquet = (PaquetGameConnection*) pPaquet;

    if(wPaquet->getConnectionState() == GAME_CONNECTION_MATCHMAKING_REQUEST)
    {
        auto wGameIdentifier = GameServerManager::obtenirInstance()->doMatchmaking(wPaquet->getUsername());
        unsigned int wGameServerId = wGameIdentifier.first;
        if(wGameServerId == 0)
        {
            wPaquet->setConnectionState(GAME_CONNECTION_MATCHMAKING_FAILED);
        }
        else
        {
            GameServer* wGameServer = GameServerManager::obtenirInstance()->getGameServer(wGameServerId);
            int wGameId = wGameIdentifier.second;
            wPaquet->setGameServerId(wGameServerId);
            wPaquet->setGameId(wGameIdentifier.second);
            wPaquet->setGameServerIp(wGameServer->getServerIP());
            wPaquet->setMapName(wGameServer->getGame(wGameId)->getMapName());
            wPaquet->setConnectionState(GAME_CONNECTION_MATCHMAKING_REPLY);
        }
    }
    else
    {
        GameServer* wServer = GameServerManager::obtenirInstance()->getGameServer(wPaquet->getGameServerId());
        if(wServer)
        {
            wPaquet->setGameServerIp(wServer->getServerIP());
            wPaquet->setConnectionState(GAME_CONNECTION_REPLY_GAME_SERVER_IP);
        }
        else
        {
            wPaquet->setGameId(-1);
            wPaquet->setConnectionState(GAME_CONNECTION_REJECTED);
        }
    }

    GestionnaireReseau::obtenirInstance()->envoyerPaquet(wPaquet->getUsername(), wPaquet, TCP);
    return 0;
}



int PaquetRunnable::RunnableGameStatusMasterServer( Paquet* pPaquet )
{
    PaquetGameStatus* wPaquet = (PaquetGameStatus*) pPaquet;

    const int wGameId = wPaquet->getGameInfos()->getUniqueGameId();
    const int wServerId = wPaquet->getGameInfos()->getServerId();

    GameServer* wGameServer = GameServerManager::obtenirInstance()->getGameServer(wServerId);
    if(wGameServer)
    {
        PartieServeurs* wGame = wGameServer->getGame(wGameId);
        if(wGame)
        {
            wGame->updateData(wPaquet->getGameInfos());
        }
    }
    GameServerManager::obtenirInstance()->callUpdateCallbackFunction(wPaquet->getGameInfos()->getServerId(), wPaquet->getGameInfos()->getUniqueGameId(), wPaquet->getGameInfos()->getGameStatus());

    return 0;
}


