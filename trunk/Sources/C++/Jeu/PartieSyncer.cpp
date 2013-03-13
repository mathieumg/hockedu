//////////////////////////////////////////////////////////////////////////////
/// @file PartieSyncer.cpp
/// @author Mathieu Parent
/// @date 2013-03-13
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "PartieSyncer.h"
#include "..\Reseau\Paquets\PaquetMaillet.h"
#include "..\Reseau\GestionnaireReseau.h"
#include "RazerGameTypeDef.h"
#include "JoueurAbstrait.h"
#include "NoeudMaillet.h"


PartieSyncer::PartieSyncer( int pGameId, clock_t pFrequencyPerSec, SPJoueurAbstrait pPlayer1, SPJoueurAbstrait pPlayer2 )
{
    mLastTick = clock();
    mInterval = 1000/pFrequencyPerSec; // Arrondi au ms pres
    setPlayers(pPlayer1, pPlayer2);
    mGameId = pGameId;
}

PartieSyncer::~PartieSyncer( void )
{

}





bool PartieSyncer::isReadyToUpdate()
{
    clock_t wElapsed = clock()-mLastTick;
    if(wElapsed > mInterval)
    {
        mLastTick = clock();
        return true;
    }
    else
    {
        return false;
    }
}




void PartieSyncer::tick()
{
    if(isReadyToUpdate())
    {
        PaquetMaillet* wPaquet = (PaquetMaillet*) GestionnaireReseau::obtenirInstance()->creerPaquet(MAILLET);
        wPaquet->setPosition(mPlayerToSync->getControlingMallet()->getPosition());
        wPaquet->setGameId(mGameId);
        GestionnaireReseau::obtenirInstance()->envoyerPaquet("GameServer", wPaquet, TCP);
    }
}



void PartieSyncer::setPlayers( SPJoueurAbstrait pPlayer1, SPJoueurAbstrait pPlayer2 )
{
    // On trouve le joueur network
    if(pPlayer1->obtenirType() != JOUEUR_NETWORK)
    {
        // Trouve
        mPlayerToSync = pPlayer1;
    }
    else
    {
        if(pPlayer2->obtenirType() != JOUEUR_NETWORK)
        {
            // Trouve
            mPlayerToSync = pPlayer2;

        }
        else
        {
            // Pas la, on ne fait rien
            mPlayerToSync = NULL;
        }
    }




}






///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


