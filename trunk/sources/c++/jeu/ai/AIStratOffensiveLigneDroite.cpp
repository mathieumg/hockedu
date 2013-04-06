//////////////////////////////////////////////////////////////////////////////
/// @file AIStratOffensiveLigneDroite.cpp
/// @author Mathieu Parent
/// @date 2013-03-31
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "AIStratOffensiveLigneDroite.h"
#include "NoeudMaillet.h"
#include "Terrain.h"
#include "NoeudRondelle.h"
#include "AIMaillet.h"
#include "JoueurVirtuel.h"
#include "FacadeModele.h"
#include "Partie.h"
#include "NoeudBut.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn AIStratOffensiveLigneDroite::AIStratOffensiveLigneDroite()
///
/// Constructeur.
///
/// @param[in].
///
/// @return
///
////////////////////////////////////////////////////////////////////////
AIStratOffensiveLigneDroite::AIStratOffensiveLigneDroite(const AIMaillet& context):AIStratOffensiveRenforcement(context)
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn AIStratOffensiveLigneDroite::~AIStratOffensiveLigneDroite()
///
/// Destructeur.
///
/// @param[in].
///
/// @return
///
////////////////////////////////////////////////////////////////////////
AIStratOffensiveLigneDroite::~AIStratOffensiveLigneDroite()
{
}

void AIStratOffensiveLigneDroite::calculateTagetPos()
{
    NoeudBut* wButs[2];
    FacadeModele::getInstance()->obtenirPartieCourante()->getField()->getGoals(wButs);
    if(context_.obtenirJv()->getPlayerSide() == PLAYER_SIDE_LEFT)
    {
        setPointVise(wButs[1]->getPosition().convertir<2>());
    }
    else
    {
        setPointVise(wButs[0]->getPosition().convertir<2>());
    }

}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


