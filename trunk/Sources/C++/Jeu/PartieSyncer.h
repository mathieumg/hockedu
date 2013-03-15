//////////////////////////////////////////////////////////////////////////////
/// @file PartieSyncer.h
/// @author Mathieu Parent
/// @date 2013-03-13
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <time.h>
#include "RazerGameTypeDef.h"


///////////////////////////////////////////////////////////////////////////
/// @class PartieSyncer
/// @brief Objet pour la synchronisation de la partie sur le network.
/// Permet d'envoyer des informations a une frequence precise sans dependre du tick de la partie.
/// A comme responsabilite d'envoyer les informations de maillet
///
/// @author Mathieu Parent
/// @date 2013-03-13
///////////////////////////////////////////////////////////////////////////
class PartieSyncer
{
public:
    PartieSyncer(int pGameId, clock_t pFrequencyPerSec, SPJoueurAbstrait pPlayer1, SPJoueurAbstrait pPlayer2);
	/// Destructeur
	~PartieSyncer(void);

    // Envoie les infos necessaires au network
    void tick();

    void setPlayers(SPJoueurAbstrait pPlayer1, SPJoueurAbstrait pPlayer2);

private:

    // Returns true if the elapsed time is enough from last clock
    bool isReadyToUpdate();

    SPJoueurAbstrait mPlayerToSync;
    
    clock_t mLastTick; // Last tick time
    clock_t mInterval; // Interval between ticks

    int mGameId;

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


