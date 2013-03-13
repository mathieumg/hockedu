#pragma once
#include "Paquet.h"

class UsinePaquetGameCreation;

class PaquetGameCreation : public Paquet
{
public:
    ~PaquetGameCreation(void);

    virtual PacketTypes getOperation() const { return GAME_CREATION_REQUEST; }

    void setGameName(std::string& pGameName) { mGameName = pGameName; }
    std::string getGameName() { return mGameName; }

    void setUsername (std::string& pUsername) { mUsername = pUsername; }
    std::string getUsername() { return mUsername; }

protected:
    PaquetGameCreation(void);

private:
    friend class UsinePaquetGameCreation;
    std::string mGameName;
    std::string mUsername;
};

