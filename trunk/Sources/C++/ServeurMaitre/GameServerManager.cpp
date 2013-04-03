#include "GameServerManager.h"
#include "..\Reseau\ExceptionsReseau\ExceptionReseau.h"
#include "GameServer.h"

SINGLETON_DECLARATION_CPP(GameServerManager);
unsigned int GameServerManager::mNewGameServerId = 0;
unsigned int GameServerManager::mAddedGameServersAmount = 0;

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

unsigned int GameServerManager::selectRandomGameServer()
{
    // Evite un crash si size == 0
    if(mGameServersList.size() == 0)
    {
        return -1;
    }

    // Obtain a random index in range 0 - size
    unsigned int wIndex = rand() % mGameServersList.size();

    // Get the nth element of the list where n = index
    auto it = mGameServersList.begin();
    for(unsigned int i = 0; i < wIndex; ++i, ++it) {}
    
    // Return server id at index n.
    return it->first;
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
