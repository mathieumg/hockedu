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


///////////////////////////////////////////////////////////////////////////
/// @class PartieServeurs
/// @brief Classe qui represente une partie en cours (pour sauvagarder les informations des parties en cours sur les serveurs)
///
/// @author Mathieu Parent
/// @date 2013-02-11
///////////////////////////////////////////////////////////////////////////
class PartieServeurs {
private:
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

public:
	PartieServeurs(const std::string& pPlayer1Name, const std::string& pPlayer2Name);
	~PartieServeurs();

    

    std::string getPlayer1Name() const { return mPlayer1Name; }
    std::string getPlayer2Name() const { return mPlayer2Name; }

    int getPlayer1Score() const { return mPlayer1Score; }
    void setPlayer1Score(int val) { mPlayer1Score = val; }

    int getPlayer2Score() const { return mPlayer2Score; }
    void setPlayer2Score(int val) { mPlayer2Score = val; }

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

