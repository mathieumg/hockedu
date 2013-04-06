//////////////////////////////////////////////////////////////////////////////
/// @file AIStratOffensiveDroite.cpp
/// @author Mathieu Parent
/// @date 2013-03-31
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "AIStratOffensiveDroite.h"
#include "NoeudMaillet.h"
#include "Terrain.h"
#include "NoeudRondelle.h"
#include "AIMaillet.h"
#include "JoueurVirtuel.h"
#include "NoeudBut.h"
#include "FacadeModele.h"
#include "Partie.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn AIStratOffensiveDroite::AIStratOffensiveDroite()
///
/// Constructeur.
///
/// @param[in].
///
/// @return
///
////////////////////////////////////////////////////////////////////////
AIStratOffensiveDroite::AIStratOffensiveDroite(const AIMaillet& context):AIStratOffensiveRenforcement(context)
{
	tirReussi_ = (unsigned int)(rand() % 100 + 1) > context_.obtenirJv()->obtenirProbabiliteEchec();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn AIStratOffensiveDroite::~AIStratOffensiveDroite()
///
/// Destructeur.
///
/// @param[in].
///
/// @return
///
////////////////////////////////////////////////////////////////////////
AIStratOffensiveDroite::~AIStratOffensiveDroite()
{
}

void AIStratOffensiveDroite::calculateTagetPos()
{
    // IMPLEMENT THIS
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


