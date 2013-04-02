#pragma once
#include "Paquet.h"

class UsinePaquetGameCreation;

class PaquetGameCreation : public Paquet
{
public:
    ~PaquetGameCreation(void);

    virtual PacketTypes getOperation() const { return GAME_CREATION_REQUEST; }

    inline void setGameName(const std::string& pGameName) { mGameName = pGameName; }
    inline const std::string& getGameName() const { return mGameName; }

    inline void setUsername (const std::string& pUsername) { mUsername = pUsername; }
    inline const std::string& getUsername() const { return mUsername; }

    inline void setMapName (const std::string& pMapName) { mMapName = pMapName; }
    inline const std::string& getMapName () const { return mMapName; }

    inline void setPassword (const std::string& pPassword) { mPassword = pPassword; }
    inline const std::string& getPassword () const { return mPassword; }

    inline void setServerIP (const std::string& pServerIP) { mServerIP = pServerIP; }
    inline const std::string& getServerIP () const { return mServerIP; }

    inline void setGameId(const int& pGameId) { mGameId = pGameId; }
    inline const int& getGameId() const { return mGameId; }

    inline void setServerId(const unsigned int& pServerId) { mServerId = pServerId; }
    inline const unsigned int& getServerId() const { return mServerId; }

protected:
    PaquetGameCreation(void);

private:
    friend class UsinePaquetGameCreation;
    std::string mGameName;
    std::string mUsername;
    std::string mMapName;
    std::string mPassword;
    std::string mServerIP;
    int mGameId;
    unsigned int mServerId;
};

