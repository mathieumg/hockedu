#include "FieldRunnableStructs.h"

#if WIN32
#include "SoundFMOD.h"
#endif

#include "NoeudRondelle.h"
#include "NoeudPortail.h"
#include "Partie.h"
#include "NodeBonus.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn void FieldRunnablePortal::execute()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FieldRunnablePortal::execute()
{
    portal->setIsAttractionFieldActive(false);
    auto newPos = portal->getPosition();
    puck->setPosition(newPos);
#if WIN32
    SoundFMOD::obtenirInstance()->playEffect(effect(PORTAL_EFFECT));
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FieldRunnableGoals::execute()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FieldRunnableGoals::execute()
{
    if(game)
    {
        auto position = puck->getPosition();
        if(position[VX] < 0)
        {
            game->incrementerPointsJoueurDroit();
        }
        else
        {
            game->incrementerPointsJoueurGauche();
        }
#if WIN32
        SoundFMOD::obtenirInstance()->playEffect(GOAL_EFFECT);
#endif
    }
    game->miseAuJeu();
    puck->setCollisionDetected(false);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FieldRunnableBonus::execute()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FieldRunnableBonus::execute()
{
    bonus->ExecuteBonus(puck); 
}
