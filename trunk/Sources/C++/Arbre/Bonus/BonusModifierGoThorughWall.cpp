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
#include "NoeudAbstrait.h"
#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif

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
    mTimeToLive(10.f)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool BonusModifierGoThroughWall::Attach( NoeudAbstrait* pNode )
///
/// /*Description*/
///
/// @param[in] NoeudAbstrait * pNode
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool BonusModifierGoThroughWall::Attach( NoeudAbstrait* pNode )
{
    mOwner = pNode;
    if(mOwner)
    {
        auto existingModifiers = mOwner->GetModifiers();
        // recherche pour un bonus déjà présent
        for(auto it = existingModifiers.begin(); it != existingModifiers.end(); ++it)
        {
            BonusModifierGoThroughWall* modifier = dynamic_cast<BonusModifierGoThroughWall*>(*it);
            if(modifier)
            {
                // resets time to live on the modifier
                modifier->mTimeToLive = mTimeToLive;
                /// return false to indicate that the bonus was not added to the node, even though the 
                /// bonus itself was applied.
                return false;
            }
        }

        /// if the bonus isn't already present
        if(mOwner->AddModifier(this))
        {
            /// apply the modification on the owner
            return Apply();
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool BonusModifierGoThroughWall::Apply()
///
/// /*Description*/
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool BonusModifierGoThroughWall::Apply()
{
    if(mOwner)
    {
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
/// /*Description*/
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

////////////////////////////////////////////////////////////////////////
///
/// @fn void BonusModifierGoThroughWall::Tick( float temps )
///
/// /*Description*/
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void BonusModifierGoThroughWall::Tick( float temps )
{
    mTimeToLive -= temps;
    if(mTimeToLive < 0)
    {
        Revert();
        Complete();
    }
}
