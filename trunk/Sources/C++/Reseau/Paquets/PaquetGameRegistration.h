#pragma once
#include "Paquet.h"

class PaquetGameRegistration : public Paquet
{
public:
    ~PaquetGameRegistration(void);

    virtual PacketTypes getOperation() const { return GAME_REGISTRATION; }

    inline void setGameName ( std::string& pGameName ) { mGameName = pGameName; }
    inline std::string getGameName () const { return mGameName; }

    inline void setUsername ( std::string& pUsername ) { mUsername = pUsername; }
    inline std::string getUsername () const { return mUsername; }

    inline void setMapName(std::string val) { mMapName = val; }
    inline std::string getMapName() const { return mMapName; }

    inline void setGameId(int pGameId) { mGameId = pGameId; }
    inline int getGameId() const { return mGameId; }

    inline void setServerId (unsigned int pServerId) { mServerId = pServerId; }
    inline unsigned int getServerId () const { return mServerId; }

protected:
    PaquetGameRegistration(void);

private:
    friend class UsinePaquetGameRegistration;

    std::string mGameName;
    std::string mUsername;
    std::string mMapName;
    int mGameId;
    unsigned int mServerId;
};

