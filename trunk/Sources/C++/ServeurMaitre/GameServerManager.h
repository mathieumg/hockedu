#pragma once
#include "Singleton.h"
#include <hash_map>

class GameServer;

class GameServerManager: public Singleton<GameServerManager>
{
SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(GameServerManager);

public:
    void addNewGameServer(unsigned int pGameServerId, std::string pServerIP);
    void removeGameServer(unsigned int pGameServerId);

    GameServer* getGameServer( unsigned int pGameServerId );

    static unsigned int generateNewGameServerId() { return ++mNewGameServerId; }
    static unsigned int getLatestGameServerId() { return mNewGameServerId; }
    static unsigned int getAddedGameServersAmount() { return mAddedGameServersAmount; }
private:
    GameServerManager();
    ~GameServerManager();
    void addNewGameServer(unsigned int pGameServerId, GameServer* pGameServer);
    std::hash_map<unsigned int, GameServer*> mGameServersList;
    static unsigned int mAddedGameServersAmount;
    static unsigned int mNewGameServerId;
};

