//////////////////////////////////////////////////////////////////////////////
/// @file PartieServeurs.h
/// @author Mathieu Parent
/// @date 2013-02-11
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <time.h>
#include <string>

enum GameStatus {
    GAME_NOT_READY, 
    GAME_READY, 
    GAME_ENDED, 
    GAME_STARTED, 
    GAME_SCORE, 
    GAME_WAITING, 
    GAME_RUNNING, 
    GAME_PAUSED,
    GAME_REPLAYING,
};

// Params: (serverId, gameId, GameStatus)
typedef void (*PartieServeursCallback) (int, int, GameStatus);

///////////////////////////////////////////////////////////////////////////
/// @class PartieServeurs
/// @brief Classe qui represente une partie en cours (pour sauvagarder les informations des parties en cours sur les serveurs)
///
/// @author Mathieu Parent
/// @date 2013-02-11
///////////////////////////////////////////////////////////////////////////
class PartieServeurs {
private:
    friend class PaquetGameStatus;

    // Id du serveur
    unsigned int mServerId;

    // Id de la partie
    int mUniqueGameId;
    
    // Noms des joueurs
    std::string mPlayer1Name;
    std::string mPlayer2Name;

    //Terrain associé à la partie
    std::string mMapName;

    // Nom de la partie
    std::string mGameName;

    std::string mPassword;

    // Score des joueurs
    int mPlayer1Score;
    int mPlayer2Score;

    // Temps restant a la partie
    time_t mTime;

    // GameStatus
    GameStatus mGameStatus;
	
    static int compteurGameId;

    // Callback appelee quand le score change
    PartieServeursCallback mUpdateCallback;

    void callUpdateCallbackFunction() {if(mUpdateCallback) mUpdateCallback(mServerId, mUniqueGameId, mGameStatus);}

public:
	PartieServeurs(const std::string& pPlayer1Name, const std::string& pPlayer2Name);
    PartieServeurs();
    PartieServeurs( PartieServeurs* pSource);
	~PartieServeurs();

    /// Gets the value of ServerId
    unsigned int getServerId() const { return mServerId; }
    /// Sets the value of ServerId
    void setServerId(const unsigned int& pServerId) { mServerId = pServerId; }

    int getUniqueGameId() const { return mUniqueGameId; }
    void setUniqueGameId(const int& pUniqueGameId) { mUniqueGameId = pUniqueGameId; }  

    inline void setPlayerName1( const std::string& pPlayerName1 ) {mPlayer1Name = pPlayerName1; callUpdateCallbackFunction();}
    inline void setPlayerName2( const std::string& pPlayerName2 ) {mPlayer2Name = pPlayerName2; callUpdateCallbackFunction();}

    std::string getPlayer1Name() const { return mPlayer1Name; }
    std::string getPlayer2Name() const { return mPlayer2Name; }

    std::string getPassword() const { return mPassword; }
    void setPassword(const std::string& pPassword) { mPassword = pPassword; }

    std::string getMapName() const { return mMapName; }
    void setMapName(std::string pMapName) { mMapName = pMapName; }

    std::string getGameName() const { return mGameName; }
    void setGameName(std::string pGameName) { mGameName = pGameName; }

    int getPlayer1Score() const { return mPlayer1Score; }
    void setPlayer1Score(int val) { mPlayer1Score = val; callUpdateCallbackFunction();}

    int getPlayer2Score() const { return mPlayer2Score; }
    void setPlayer2Score(int val) { mPlayer2Score = val; callUpdateCallbackFunction();}

    time_t getTime() const { return mTime; }
    inline void setTime(time_t val) { mTime = val; callUpdateCallbackFunction();}
    void setTime(int pHours, int pMins, int pSec);

    inline GameStatus getGameStatus() const { return mGameStatus; }
	inline void setGameStatus(GameStatus val) { mGameStatus = val; callUpdateCallbackFunction();}

    void updateData(PartieServeurs* pUpdateData);

    inline void setUpdateCallback(PartieServeursCallback pCallback) {mUpdateCallback = pCallback;}
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

