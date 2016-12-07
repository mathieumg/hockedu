#pragma once
#include "Singleton.h"
#ifdef __linux__
#include <ext/hash_map>
namespace std {using namespace __gnu_cxx; }
#elif defined(WIN32)
#include <unordered_map>
#endif
#include "../Reseau/ObjetsGlobaux/PartieServeurs.h"


class GameServer;
typedef std::unordered_map<unsigned int, GameServer*> GameServersContainer;

class GameServerManager: public Singleton<GameServerManager>
{
SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(GameServerManager);

public:
    void addNewGameServer(unsigned int& pGameServerId, std::string& pServerIP, std::string& pServerIdentifier);
    void removeGameServer(unsigned int pGameServerId);

    GameServer* getGameServer( unsigned int pGameServerId );

    unsigned int selectGameServer();

    const GameServersContainer& getGameServersContainer() const { return mGameServersList; }

    static unsigned int generateNewGameServerId() { return ++mNewGameServerId; }
    static unsigned int getLatestGameServerId() { return mNewGameServerId; }
    static unsigned int getAddedGameServersAmount() { return mAddedGameServersAmount; }

    void callUpdateCallbackFunction(int pServerId, int pGameId, GameStatus pGameStatus) {if(mUpdateCallback) mUpdateCallback(pServerId, pGameId, pGameStatus);}
    inline void setUpdateCallback(PartieServeursCallback pCallback) {mUpdateCallback = pCallback;}
    std::pair<unsigned int, int> doMatchmaking(const std::string& pUsername);
private:
    GameServerManager();
    ~GameServerManager();
    void addNewGameServer(unsigned int pGameServerId, GameServer* pGameServer);
    GameServersContainer mGameServersList;
    static unsigned int mAddedGameServersAmount;
    static unsigned int mNewGameServerId;

    // Callback appelee quand le score change dans une partie
    PartieServeursCallback mUpdateCallback;
};

