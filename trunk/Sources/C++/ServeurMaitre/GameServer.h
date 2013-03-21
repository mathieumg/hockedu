#pragma once
#include <string>
#include <hash_map>

class PartieServeurs;

class GameServer
{
public:
    GameServer(unsigned int pServerId, std::string pServerIP);
    ~GameServer(void);

    void addGame(int pGameId, std::string pGameName, std::string pMapName, std::string pUsername);

    PartieServeurs* getGame(int pGameId);

    void removeGame(int pGameId);
private:
    std::string mServerIP;
    unsigned int mServerId;
    void addGame(int pGameId, PartieServeurs* pGame);
    std::hash_map<int, PartieServeurs*> mGamesList;
};

