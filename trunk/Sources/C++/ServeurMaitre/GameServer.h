#pragma once
#include <string>
#ifdef __linux__
#include <ext/hash_map>
namespace std {using namespace __gnu_cxx; }
#elif defined(WIN32)
#include <hash_map>
#endif

class PartieServeurs;
typedef std::hash_map<int, PartieServeurs*> GamesContainer;

class GameServer
{
public:
    GameServer(unsigned int pServerId, std::string& pServerIP, std::string& pServerIdentifier);
    ~GameServer(void);

    void addGame(int pGameId, std::string& pGameName, std::string& pMapName, std::string& pUsername, std::string& pPassword);

    PartieServeurs* getGame(int pGameId);

    std::string getServerIdentifier() { return mServerIdentifier; }

    std::string getServerIP() { return mServerIP; }

    const GamesContainer& getGamesContainer() const { return mGamesList; }

    void removeGame(int pGameId);
private:
    std::string mServerIP;
    std::string mServerIdentifier;
    unsigned int mServerId;
    void addGame(int pGameId, PartieServeurs* pGame);
    GamesContainer mGamesList;
};

