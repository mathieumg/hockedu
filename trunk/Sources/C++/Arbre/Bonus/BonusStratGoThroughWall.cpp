///////////////////////////////////////////////////////////////////////////////
/// @file BonusStratGoThroughWall.cpp
/// @author Michael Ferris
/// @date 2013-03-14
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "BonusStratGoThroughWall.h"
#include "NoeudRondelle.h"
#include "BonusModifierGoThorughWall.h"
#include "NoeudMaillet.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn void BonusStratGoThroughWall::Execute( class NoeudRondelle* pRondelle )
///
/// /*Description*/
///
/// @param[in] class NoeudRondelle * pRondelle
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void BonusStratGoThroughWall::Execute( NoeudRondelle* pRondelle )
{
    auto maillet = pRondelle->getLastHittingMallet();
    if(maillet)
    {
        BonusModifierGoThroughWall* modifier = new BonusModifierGoThroughWall();
        if(!modifier->Attach(maillet))
        {
            // the modifier couldn't attach itself on the node so we delete it
            delete modifier;
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void BonusStratGoThroughWall::Tick( float temps )
///
/// /*Description*/
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void BonusStratGoThroughWall::Tick( float temps )
{

}

