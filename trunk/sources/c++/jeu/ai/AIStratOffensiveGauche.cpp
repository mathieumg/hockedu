//////////////////////////////////////////////////////////////////////////////
/// @file AIStratOffensiveGauche.cpp
/// @author Mathieu Parent
/// @date 2013-03-31
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "AIStratOffensiveGauche.h"
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
/// @fn AIStratOffensiveGauche::AIStratOffensiveGauche()
///
/// Constructeur.
///
/// @param[in].
///
/// @return
///
////////////////////////////////////////////////////////////////////////
AIStratOffensiveGauche::AIStratOffensiveGauche(const AIMaillet& context):AIStratOffensiveRenforcement(context)
{
	tirReussi_ = (unsigned int)(rand() % 100 + 1) > context_.obtenirJv()->obtenirProbabiliteEchec();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn AIStratOffensiveGauche::~AIStratOffensiveGauche()
///
/// Destructeur.
///
/// @param[in].
///
/// @return
///
////////////////////////////////////////////////////////////////////////
AIStratOffensiveGauche::~AIStratOffensiveGauche()
{
}

void AIStratOffensiveGauche::calculateTagetPos()
{
    NoeudBut* wButs[2];
    FacadeModele::getInstance()->obtenirPartieCourante()->getField()->getGoals(wButs);
    Vecteur2 wPointVise;
    if(context_.obtenirJv()->getPlayerSide() == PLAYER_SIDE_LEFT)
    {
        Vecteur2 wPosBut = wButs[1]->getPosition().convertir<2>();
        wPointVise = Vecteur2(wPosBut[VX]/2.0f, wPosBut[VY]-(0.5f * wPosBut[VX]));
    }
    else
    {
        Vecteur2 wPosBut = wButs[0]->getPosition().convertir<2>();
        wPointVise = Vecteur2(wPosBut[VX]/2.0f, wPosBut[VY]+(0.5f * wPosBut[VX]));
    }
    setPointVise(wPointVise);
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


