#include "GameServer.h"
#include "..\Reseau\ExceptionsReseau\ExceptionReseau.h"
#include "..\Reseau\ObjetsGlobaux\PartieServeurs.h"





////////////////////////////////////////////////////////////////////////
///
/// @fn GameServer::GameServer( unsigned int pServerId, std::string pServerIP ) : mServerId(pServerId), mServerIP(pServerIP)
///
/// Constructor
///
/// @param[in] unsigned int pServerId The server's ID.
/// @param[in] std::string& pServerIP The server's IP.
/// @param[in] std::string& pServerIdentifier The server's identifier in the socket map.

/// @return
///
////////////////////////////////////////////////////////////////////////
GameServer::GameServer(unsigned int pServerId, std::string& pServerIP, std::string& pServerIdentifier)
    : mServerId(pServerId), mServerIP(pServerIP), mServerIdentifier(pServerIdentifier)
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn GameServer::~GameServer()
///
/// Destructor - ensures every Game was removed from the container.
///
///
/// @return
///
////////////////////////////////////////////////////////////////////////
GameServer::~GameServer()
{
    for(auto it = mGamesList.begin(); it != mGamesList.end(); ++it)
    {
        removeGame(it->first);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn GameServer::addGame( int pGameId, std::string pGameName, std::string pMapName, std::string pUsername )
///
/// Adds a game to the GameServer using the specified parameters.
///
/// @param[in] int pGameId The ID of the game to add.
/// @param[in] std::string pGameName The name of the game.
/// @param[in] std::string pMapName The map used by the game.
/// @param[in] std::string pUsername The username of the user who created the game.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameServer::addGame( const int& pGameId, const std::string& pGameName, const std::string& pMapName, const std::string& pUsername, const std::string& pPassword )
{
    PartieServeurs* wGame = new PartieServeurs(pUsername, "");
    wGame->setGameName(pGameName);
    wGame->setMapName(pMapName);
    wGame->setPassword(pPassword);
    wGame->setServerId(mServerId);
    wGame->setUniqueGameId(pGameId);
    addGame(pGameId, wGame);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn GameServer::addGame( int pGameId, PartieServeurs* pGame )
///
/// Adds a given game to the GameServer.
///
/// @param[in] int pGameId The ID of the game to add.
/// @param[in] PartieServeurs * pGame The game to add.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameServer::addGame( int pGameId, PartieServeurs* pGame )
{
    auto it = mGamesList.find(pGameId);

    if(it != mGamesList.end())
    {
        throw ExceptionReseau("The specified game ID already exists on this server.");
    }

    mGamesList[pGameId] = pGame;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn GameServer::getGame( int pGameId )
///
/// Returns the game with the specified game ID.
///
/// @param[in] int pGameId The game ID of the game we want.
///
/// @return PartieServeurs* The game with the specified game ID.
///
////////////////////////////////////////////////////////////////////////
PartieServeurs* GameServer::getGame( int pGameId )
{
    auto it = mGamesList.find(pGameId);

    if(it == mGamesList.end())
    {
        throw ExceptionReseau("The specified game ID doesn't exist on this server.");
    }

    return mGamesList[pGameId];
}

////////////////////////////////////////////////////////////////////////
///
/// @fn GameServer::removeGame( int pGameId )
///
/// Removes the game with the specified game id.
///
/// @param[in] int pGameId The ID of the game we want to remove.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameServer::removeGame( int pGameId )
{
    auto it = mGamesList.find(pGameId);

    if(it == mGamesList.end())
    {
        throw ExceptionReseau("The specified game ID doesn't exist on this server.");
    }

    delete (*it).second;
    mGamesList.erase((*it).first);
}
