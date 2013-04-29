///////////////////////////////////////////////////////////////////////////////
/// @file BonusModifierChangeZone.cpp
/// @author Michael Ferris
/// @date 2013-03-29
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "BonusModifierChangeZone.h"

// ne fait pas de sens d'utiliser un modifier si on ne joue pas
#if !SHIPPING
#include <iostream>
#endif
#if BOX2D_PLAY  
#include <Box2D/Box2D.h>
#endif
#if WIN32
#include "SoundFMOD.h"
#endif
#include "NoeudAbstrait.h"
#include "Utilitaire.h"
#include "NoeudRondelle.h"
#include "NoeudMaillet.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  BonusModifierChangeZone::BonusModifierChangeZone()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
BonusModifierChangeZone::BonusModifierChangeZone():
    BonusModifierAbstract(10)
{
}



////////////////////////////////////////////////////////////////////////
///
/// @fn  BonusModifierChangeZone::~BonusModifierChangeZone()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
BonusModifierChangeZone::~BonusModifierChangeZone()
{

}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool BonusModifierChangeZone::Attach( NoeudAbstrait* pPuck )
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
bool BonusModifierChangeZone::Attach( NoeudRondelle* pPuck )
{
#if WIN32
    SoundFMOD::obtenirInstance()->playEffect(BONUS_CHANGE_SIDE_IN_EFFECT);
#endif //WIN32
    return AttachToLastHittingMallet(pPuck);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool BonusModifierChangeZone::Apply()
///
/// Applies the real effect on the node
/// returns false if the bonus finished the execution (no time to live)
/// in case it returns false, complete its execution
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool BonusModifierChangeZone::Apply()
{
    if(mOwner)
    {
        auto existingModifiers = mOwner->GetModifiers();
        // recherche pour un bonus déjà présent
        for(auto it = existingModifiers.begin(); it != existingModifiers.end(); ++it)
        {
            BonusModifierChangeZone* modifier = dynamic_cast<BonusModifierChangeZone*>(*it);
            if(modifier && modifier != this)
            {
                // resets time to live on the modifier
                modifier->mTimeToLive = mTimeToLive;
                /// return false to indicate that the bonus was not added to the node, even though the 
                /// bonus itself was applied.
                return false;
            }
        }
#if BOX2D_PLAY 

        auto body = mOwner->getPhysicBody();
        if(body)
        {
            for(auto fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
            {
                b2Filter filter = fixture->GetFilterData();
                if(filter.maskBits & CATEGORY_MIDLANE)
                {
                    // removes the wall as categorie that can block the node
                    filter.maskBits &= ~CATEGORY_MIDLANE;
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
/// @fn bool BonusModifierChangeZone::Revert()
///
/// Reverts the effect on the node,
/// return unsed for now
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool BonusModifierChangeZone::Revert()
{
#if WIN32 
    SoundFMOD::obtenirInstance()->playEffect(BONUS_CHANGE_SIDE_OUT_EFFECT); 
#endif

#if BOX2D_PLAY  
    for(int i=0; i<(int)mFixtures.size(); ++i)
    {
        auto fixture = mFixtures[i];
        b2Filter filter = fixture->GetFilterData();
        // adds the wall as categories that can block the node
        filter.maskBits |= CATEGORY_MIDLANE;
        fixture->SetFilterData(filter);
    }
#endif
    NoeudMaillet* maillet = (NoeudMaillet*)mOwner;
    if(maillet)
    {
        auto pos = maillet->getPosition();
        auto initialPos = maillet->getOriginalPosition();
        // verification si le maillet n'est plus dans sa zone
        if(pos[VX]*initialPos[VX] <= 0)
        {
            maillet->setPosition(initialPos);
        }
    }
    return true;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void BonusModifierChangeZone::render()
///
/// Render specific content for this modifier
/// translation to the node will already be applied
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void BonusModifierChangeZone::render() const
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void BonusModifierChangeZone::Tick( float temps )
///
/// Tick the modifier
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void BonusModifierChangeZone::Tick( float temps )
{
    Super::Tick(temps);
}


////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////