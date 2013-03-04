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

typedef int (*PartieServeursCallback) ();

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

    // Id de la partie
    int mUniqueGameId;
    
    // Noms des joueurs
    std::string mPlayer1Name;
    std::string mPlayer2Name;

    // Score des joueurs
    int mPlayer1Score;
    int mPlayer2Score;

    // Temps restant a la partie
    time_t mTime;
    
    static int compteurGameId;

    // Callback appelee quand le score change
    PartieServeursCallback mUpdateCallback;

    void callUpdateCallbackFunction() {if(mUpdateCallback) mUpdateCallback();}

public:
	PartieServeurs(const std::string& pPlayer1Name, const std::string& pPlayer2Name);
    PartieServeurs();
    PartieServeurs( PartieServeurs* pSource);
	~PartieServeurs();

    int getUniqueGameId() const { return mUniqueGameId; }

    inline void setPlayerName1( const std::string& pPlayerName1 ) {mPlayer1Name = pPlayerName1;}
    inline void setPlayerName2( const std::string& pPlayerName2 ) {mPlayer2Name = pPlayerName2;}

    std::string getPlayer1Name() const { return mPlayer1Name; }
    std::string getPlayer2Name() const { return mPlayer2Name; }

    int getPlayer1Score() const { return mPlayer1Score; }
    void setPlayer1Score(int val) { mPlayer1Score = val; callUpdateCallbackFunction();}

    int getPlayer2Score() const { return mPlayer2Score; }
    void setPlayer2Score(int val) { mPlayer2Score = val; callUpdateCallbackFunction();}

    time_t getTime() const { return mTime; }
    inline void setTime(time_t val) { mTime = val; }
    void setTime(int pHours, int pMins, int pSec);

    inline void setUpdateCallback(PartieServeursCallback pCallback) {mUpdateCallback = pCallback;}
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

