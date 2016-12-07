#pragma once
#include <string>
#ifdef __linux__
#include <ext/hash_map>
namespace std {using namespace __gnu_cxx; }
#elif defined(WIN32)
#include <unordered_map>
#endif

class PartieServeurs;
typedef std::unordered_map<int, PartieServeurs*> GamesContainer;

class GameServer
{
public:
    GameServer(unsigned int pServerId, std::string& pServerIP, std::string& pServerIdentifier);
    ~GameServer(void);

    void addGame(const int& pGameId, const std::string& pGameName, const std::string& pMapName, const std::string& pUsername, const std::string& pPassword);

    PartieServeurs* getGame(int pGameId);

    const std::string& getServerIdentifier() { return mServerIdentifier; }

    const std::string& getServerIP() { return mServerIP; }

    const unsigned int& getServerId() { return mServerId; }

    const GamesContainer& getGamesContainer() const { return mGamesList; }

    void removeGame(int pGameId);
private:
    std::string mServerIP;
    std::string mServerIdentifier;
    unsigned int mServerId;
    void addGame(int pGameId, PartieServeurs* pGame);
    GamesContainer mGamesList;
};

