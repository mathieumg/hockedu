///////////////////////////////////////////////////////////////////////////////
/// @file BonusModifierGoThroughWall.cpp
/// @author Michael Ferris
/// @date 2013-03-14
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "BonusModifierGoThorughWall.h"
#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif
#include "NoeudAbstrait.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  BonusModifierGoThroughWall::BonusModifierGoThroughWall()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
BonusModifierGoThroughWall::BonusModifierGoThroughWall():
    BonusModifierAbstract(10)
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool BonusModifierGoThroughWall::Attach( NoeudAbstrait* pPuck )
///
/// Attach a modifier on a node, receiving the puck as entry point,
/// but from it, the modifier can apply itself on anything in the map
///
/// returns true if the modifier is attached on a node
/// returns false otherwise
/// note, returning false doesn't mean the bonus was not applied or refreshed
///
/// @param[in] NoeudAbstrait * pPuck
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool BonusModifierGoThroughWall::Attach( NoeudRondelle* pPuck )
{
    return AttachToLastHittingMallet(pPuck);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool BonusModifierGoThroughWall::Apply()
///
/// Applies the real effect on the node
/// returns false if the bonus finished the execution (no time to live)
/// in case it returns false, complete its execution
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool BonusModifierGoThroughWall::Apply()
{
    if(mOwner)
    {
        auto existingModifiers = mOwner->GetModifiers();
        // recherche pour un bonus déjà présent
        for(auto it = existingModifiers.begin(); it != existingModifiers.end(); ++it)
        {
            BonusModifierGoThroughWall* modifier = dynamic_cast<BonusModifierGoThroughWall*>(*it);
            if(modifier && modifier != this)
            {
                // resets time to live on the modifier
                modifier->mTimeToLive = mTimeToLive;
                /// return false to indicate that the bonus was not added to the node, even though the 
                /// bonus itself was applied.
                return false;
            }
        }
#if BOX2D_INTEGRATED 

        auto body = mOwner->getPhysicBody();
        if(body)
        {
            for(auto fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
            {
                b2Filter filter = fixture->GetFilterData();
                if(filter.maskBits & CATEGORY_WALL)
                {
                    // removes the wall as categorie that can block the node
                    filter.maskBits &= ~CATEGORY_WALL;
                    fixture->SetFilterData(filter);
                    mFixtures.push_back(fixture);
                }
            }
        }
#endif
    }

    // if any fixtures were found, it means that this node had a modifiation applied
    return mFixtures.size() != 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool BonusModifierGoThroughWall::Revert()
///
/// Reverts the effect on the node,
/// return unsed for now
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool BonusModifierGoThroughWall::Revert()
{
#if BOX2D_INTEGRATED  
    for(int i=0; i<(int)mFixtures.size(); ++i)
    {
        auto fixture = mFixtures[i];
        b2Filter filter = fixture->GetFilterData();
        // adds the wall as categories that can block the node
        filter.maskBits |= CATEGORY_WALL;
        fixture->SetFilterData(filter);
    }
#endif
    return true;
}
