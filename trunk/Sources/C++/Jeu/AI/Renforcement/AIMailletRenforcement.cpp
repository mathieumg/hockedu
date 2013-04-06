//////////////////////////////////////////////////////////////////////////////
/// @file AIMailletRenforcement.cpp
/// @author Mathieu Parent 
/// @date 2013-03-31
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "AIMailletRenforcement.h"
#include "NoeudRondelle.h"
#include "NoeudMaillet.h"
#include "Terrain.h"
#include "Partie.h"
#include "FacadeModele.h"
#include "AIStratOffensiveRenforcement.h"
#include "JoueurVirtuelRenforcement.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn  AIMailletRenforcement::AIMailletRenforcement( const JoueurVirtuel& jv )
///
/// Constructeur
///
/// @param[in] const JoueurVirtuel & jv : reference du joueurVirtuel ayant cet Intelligence Artificiel
///										la reference force cette instance a exister et donc non-Null
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
AIMailletRenforcement::AIMailletRenforcement(const JoueurVirtuel& jv): AIMaillet(jv)
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn AIMailletRenforcement::~AIMailletRenforcement()
///
/// Destructeur.
///
/// @return
///
////////////////////////////////////////////////////////////////////////
AIMailletRenforcement::~AIMailletRenforcement()
{
	
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void AIMailletRenforcement::evaluerStrategie( NoeudRondelle* rondelle, NoeudMaillet* maillet )
///
/// Évaluation de la stratégie à utiliser selon la position de la 
/// rondelle et le côté du maillet
///
/// @param[in] NoeudRondelle * rondelle : pointeur sur la rondelle
/// @param[in] NoeudMaillet * maillet : pointeur sur le maillet du jv
///
/// @return
///
////////////////////////////////////////////////////////////////////////
void AIMailletRenforcement::evaluerStrategie( NoeudMaillet* maillet )
{
    // On utilise la prediction pour trouver la position de la rondelle a x=75
    float wPosX = 75.0f;
    if(maillet->estAGauche())
    {
        wPosX *= -1.0f;
    }

    Partie* wGame = FacadeModele::getInstance()->obtenirPartieCourante();

    checkf(wGame);
    if(wGame)
    {
        PuckProjection wPred = wGame->getPuckProjection(wPosX, 10000);

        if(wPred.time == -1)
        {
            changerStrat(OFFENSIVE); // Ne fait que repousser la rondelle
            return;
        }

        // Get good strat
        
        changerStrat(DEFENSIVE); // Va set le point vise

        // Set important data
        AIStratOffensiveRenforcement* wStrat = (AIStratOffensiveRenforcement*) strategie_;
        wStrat->setPointImpact(wPred.position);
        wStrat->setTimeBeforeImpact(wPred.time);


    }
    else
    {
        // Si erreur, on donne une valeur arbitraire
        changerStrat(DEFENSIVE);
    }

}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



