#pragma once
#include "Paquet.h"

class UsinePaquetGameConnection;

// Game connection request states
enum GameConnectionState {
    GAME_CONNECTION_PENDING, 
    GAME_CONNECTION_ALREADY_CONNECTED, 
    GAME_CONNECTION_ACCEPTED_LEFT, 
    GAME_CONNECTION_ACCEPTED_RIGHT, 
    GAME_CONNECTION_GAME_FULL, 
    GAME_CONNECTION_WRONG_PASSWORD, 
    GAME_CONNECTION_GAME_NOT_FOUND, 
    GAME_CONNECTION_REJECTED, 
    GAME_CONNECTION_REPLY_GAME_SERVER_IP,
    GAME_CONNECTION_MATCHMAKING_REQUEST,
    GAME_CONNECTION_MATCHMAKING_FAILED,
    GAME_CONNECTION_MATCHMAKING_REPLY
};

class PaquetGameConnection : public Paquet
{
public:
    ~PaquetGameConnection(void);

    virtual PacketTypes getOperation() const { return GAME_CONNECTION; }


    inline int getGameId() const { return mGameId; }
    inline void setGameId(int val) { mGameId = val; }

    inline std::string getUsername() const { return mUsername; }
    inline void setUsername(std::string val) { mUsername = val; }

    inline std::string getPassword() const { return mPassword; }
    inline void setPassword(std::string val) { mPassword = val; }

    inline GameConnectionState getConnectionState() const { return mConnectionState; }
    inline void setConnectionState(GameConnectionState val) { mConnectionState = val; }

    inline int getGameServerId() const { return mGameServerId; }
    inline void setGameServerId(int val) { mGameServerId = val; }

    inline const std::string& getGameServerIp() const { return mGameServerIp; }
	inline void setGameServerIp(const std::string& val) { mGameServerIp = val; }

    std::string getMapName() const { return mMapName; }
    void setMapName(const std::string& pMapName) { mMapName = pMapName; }

protected:
    PaquetGameConnection(void);

private:
    friend class UsinePaquetGameConnection;

    int mGameId; // GameId of the game you want to connect to
    int mGameServerId; // GameServerId of the game you want to connect to
    std::string mUsername; // User of the player connecting
    std::string mPassword; // For future modifications
    GameConnectionState mConnectionState; // Flag for return message
    std::string mGameServerIp; // Ip of the game server
    std::string mMapName; // Map of the game
};

