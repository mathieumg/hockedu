#pragma once
#include "Paquet.h"

class UsinePaquetGameCreation;

class PaquetGameCreation : public Paquet
{
public:
    ~PaquetGameCreation(void);

    virtual PacketTypes getOperation() const { return GAME_CREATION_REQUEST; }

    inline void setGameName(const std::string& pGameName) { mGameName = pGameName; }
    inline std::string getGameName() { return mGameName; }

    inline void setUsername (const std::string& pUsername) { mUsername = pUsername; }
    inline std::string getUsername() { return mUsername; }

    inline int getGameId() const { return mGameId; }
	inline void setGameId(int val) { mGameId = val; }

protected:
    PaquetGameCreation(void);

private:
    friend class UsinePaquetGameCreation;
    std::string mGameName;
    std::string mUsername;
    int mGameId;
	
};

