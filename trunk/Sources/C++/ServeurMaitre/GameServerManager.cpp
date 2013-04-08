#include "GameServerManager.h"
#include "..\Reseau\ExceptionsReseau\ExceptionReseau.h"
#include "GameServer.h"
#include "..\Reseau\ObjetsGlobaux\PartieServeurs.h"
#include <iostream>
#include "CommunicateurBD.h"
#include <limits>

SINGLETON_DECLARATION_CPP(GameServerManager);
unsigned int GameServerManager::mNewGameServerId = 0;
unsigned int GameServerManager::mAddedGameServersAmount = 0;

void CallbackPartieServeursUpdate(int pServerId, int pGameId, GameStatus pGameStatus)
{
    if(pGameStatus == GAME_ENDED)
    {
        // Si partie terminee, on sauvegarde dans la BD le score final
        std::cout << "Game (" << pServerId << "," << pGameId << ") ended" << std::endl;


        GameServer* wGameServer = GameServerManager::obtenirInstance()->getGameServer(pServerId);
        if(wGameServer)
        {
            // Envoyer data a la BD
            PartieServeurs* wGame = wGameServer->getGame(pGameId);
            if(wGame)
            {
                bool wReturnValue = CommunicateurBD::obtenirInstance()->addGameResult(wGame->getPlayer1Name(), wGame->getPlayer2Name(), wGame->getPlayer1Score(), wGame->getPlayer2Score(), (int) wGame->getTime());
                wReturnValue ? (std::cout << "Sauvegarde dans la BD reussie." << std::endl) : (std::cout << "Sauvegarde dans la BD echouee." << std::endl);

            }
            // On supprime la partie du serveur
            wGameServer->removeGame(pGameId);
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn GameServerManager::GameServerManager()
///
/// Default constructor
///
///
/// @return
///
////////////////////////////////////////////////////////////////////////
GameServerManager::GameServerManager()
    : mGameServersList()
{
    mUpdateCallback = CallbackPartieServeursUpdate;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn GameServerManager::GameServerManager()
///
/// Destructor - ensures every GameServer was removed from the container.
///
///
/// @return
///
////////////////////////////////////////////////////////////////////////
GameServerManager::~GameServerManager()
{
    for(auto it = mGameServersList.begin(); it != mGameServersList.end(); ++it)
    {
        removeGameServer((*it).first);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn GameServerManager::addNewGameServer( unsigned int pGameServerId, GameServer* pGameServer )
///
/// Adds a new game server to the GameServer container using the specified parameters.
///
/// @param[in] unsigned int pGameServerId The ID of the game server to add.
/// @param[in] GameServer * pGameServer The object containing the information of the GameServer to add.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameServerManager::addNewGameServer( unsigned int& pGameServerId, std::string& pServerIP, std::string& pServerIdentifier )
{
    GameServer* wGameServer = new GameServer(pGameServerId, pServerIP, pServerIdentifier);
    addNewGameServer(pGameServerId, wGameServer);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn GameServerManager::addNewGameServer( unsigned int pGameServerId, GameServer* pGameServer )
///
/// Adds a new game server to the GameServer container.
///
/// @param[in] unsigned int pGameServerId The ID of the game server to add.
/// @param[in] GameServer * pGameServer The object containing the information of the GameServer to add.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameServerManager::addNewGameServer( unsigned int pGameServerId, GameServer* pGameServer )
{
    auto it = mGameServersList.find(pGameServerId);
    if ( it != mGameServersList.end())
    {
        throw ExceptionReseau("The specified game server ID already exists.");
    }

    mGameServersList[pGameServerId] = pGameServer;
    ++mAddedGameServersAmount;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn GameServerManager::removeGameServer( unsigned int pGameServerId )
///
/// Removes the specified GameServer from the GameServers map.
///
/// @param[in] unsigned int pGameServerId The ID of the game server to remove.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameServerManager::removeGameServer( unsigned int pGameServerId )
{
    auto it = mGameServersList.find(pGameServerId);
    if ( it == mGameServersList.end())
    {
        throw ExceptionReseau("The specified game server ID doesn't exist.");
    }

    delete (*it).second;
    mGameServersList.erase(it->first);
}

unsigned int GameServerManager::selectGameServer()
{
    // Evite un crash si size == 0
    if(mGameServersList.size() == 0)
    {
        return -1;
    }

    unsigned int wLowestGamesAmount = -1;
    unsigned int wServerWithLowestGamesAmount = 0;

    // Iterate over
    for(auto it = mGameServersList.begin(); it != mGameServersList.end(); ++it)
    {
        unsigned int wCurrentServerGamesAmount = it->second->getGamesContainer().size();
        if (wCurrentServerGamesAmount == 0)
        {
            wServerWithLowestGamesAmount = it->first;
            break;
        }
        else if(wCurrentServerGamesAmount < wLowestGamesAmount)
        {
            wLowestGamesAmount = wCurrentServerGamesAmount;
            wServerWithLowestGamesAmount = it->first;
        }
    }

    // Return server id at the selected index.
    return wServerWithLowestGamesAmount;
}

std::pair<unsigned int, int> GameServerManager::doMatchmaking(const std::string& pUsername)
{
    UsersWinRateContainer wWinRateContainer;
    CommunicateurBD::obtenirInstance()->getUsersWinRate(wWinRateContainer);
    float wUserWinRate(wWinRateContainer[pUsername]);
    float wLowestRateDifference(std::numeric_limits<float>::max());
    std::pair<unsigned int, int> wLowestGameIdentifier(0, -1);
    auto wGameServersContainer = getGameServersContainer();
    for(auto itGameServers = wGameServersContainer.begin(); itGameServers != wGameServersContainer.end(); ++itGameServers)
    {
        GameServer* wGameServer = itGameServers->second;
        auto wGamesContainer = wGameServer->getGamesContainer();
        for(auto itGames = wGamesContainer.begin(); itGames != wGamesContainer.end(); ++itGames)
        {
            PartieServeurs* wGame = itGames->second;
            if(wGame->getPlayer2Name() == "" && wGame->getPassword() == "")
            {
                float wCreatorRate(wWinRateContainer[wGame->getPlayer1Name()]);
                float wRateDifference = abs(wCreatorRate - wUserWinRate);
                if(wRateDifference < wLowestRateDifference)
                {
                    wLowestRateDifference = wLowestRateDifference;
                    wLowestGameIdentifier = std::pair<unsigned int, int>(wGameServer->getServerId(), wGame->getUniqueGameId());
                }
            }
        }
    }

    return wLowestGameIdentifier;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn GameServerManager::getGameServer( unsigned int pGameServerId )
///
/// Returns the specified GameServer.
///
/// @param[in] unsigned int pGameServerId The ID of the GameServer we want to get.
///
/// @return GameServer* A pointer to the specified GameServer.
///
////////////////////////////////////////////////////////////////////////
GameServer* GameServerManager::getGameServer( unsigned int pGameServerId )
{
    auto it = mGameServersList.find(pGameServerId);
    if ( it == mGameServersList.end())
    {
        throw ExceptionReseau("The specified game server ID doesn't exist.");
    }

    return it->second;
}
