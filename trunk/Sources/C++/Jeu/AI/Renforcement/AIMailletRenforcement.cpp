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
    NoeudRondelle* rondelle;
    if(!maillet->getField() || !(rondelle = maillet->getField()->getPuck()))
        return;

    (rondelle->getPosition()[VX] * maillet->getPosition()[VX] >= 0) ? changerStrat(OFFENSIVE_LIGNE_DROITE): changerStrat(DEFENSIVE);
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



