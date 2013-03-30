#pragma once
#include "Singleton.h"
#ifdef __linux__
#include <ext/hash_map>
namespace std {using namespace __gnu_cxx; }
#elif defined(WIN32)
#include <hash_map>
#endif


class GameServer;
typedef std::hash_map<unsigned int, GameServer*> GameServersContainer;

class GameServerManager: public Singleton<GameServerManager>
{
SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(GameServerManager);

public:
    void addNewGameServer(unsigned int& pGameServerId, std::string& pServerIP, std::string& pServerIdentifier);
    void removeGameServer(unsigned int pGameServerId);

    GameServer* getGameServer( unsigned int pGameServerId );

    unsigned int selectRandomGameServer();

    const GameServersContainer& getGameServersContainer() const { return mGameServersList; }

    static unsigned int generateNewGameServerId() { return ++mNewGameServerId; }
    static unsigned int getLatestGameServerId() { return mNewGameServerId; }
    static unsigned int getAddedGameServersAmount() { return mAddedGameServersAmount; }
private:
    GameServerManager();
    ~GameServerManager();
    void addNewGameServer(unsigned int pGameServerId, GameServer* pGameServer);
    GameServersContainer mGameServersList;
    static unsigned int mAddedGameServersAmount;
    static unsigned int mNewGameServerId;
};

