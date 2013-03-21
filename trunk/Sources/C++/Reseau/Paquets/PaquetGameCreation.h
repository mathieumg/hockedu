#pragma once
#include "Paquet.h"

class UsinePaquetGameCreation;

class PaquetGameCreation : public Paquet
{
public:
    ~PaquetGameCreation(void);

    virtual PacketTypes getOperation() const { return GAME_CREATION_REQUEST; }

    inline void setGameName(const std::string& pGameName) { mGameName = pGameName; }
    inline std::string getGameName() const { return mGameName; }

    inline void setUsername (const std::string& pUsername) { mUsername = pUsername; }
    inline std::string getUsername() const { return mUsername; }

    inline void setMapName (const std::string& pMapName) { mMapName = pMapName; }
    inline std::string getMapName () const { return mMapName; }

	inline void setGameId(const int& val) { mGameId = val; }
    inline int getGameId() const { return mGameId; }

protected:
    PaquetGameCreation(void);

private:
    friend class UsinePaquetGameCreation;
    std::string mGameName;
    std::string mUsername;
    std::string mMapName;
    int mGameId;
	
};

