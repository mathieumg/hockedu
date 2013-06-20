#include "FieldRunnableStructs.h"

#include "NoeudRondelle.h"
#include "NoeudPortail.h"
#include "Partie.h"
#include "NodeBonus.h"
#include "Terrain.h"

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
    }
    
    auto field = puck->getField();
    if(field)
    {
        auto pucks = field->getPucks();
        if(pucks && pucks->size() > 1)
        {
            puck->deleteThis();
            // remove puck and dont reset current game
            return;
        }
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
