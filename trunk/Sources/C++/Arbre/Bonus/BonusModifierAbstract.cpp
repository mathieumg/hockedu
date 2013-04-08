///////////////////////////////////////////////////////////////////////////////
/// @file BonusModifierAbstract.cpp
/// @author Michael Ferris
/// @date 2013-03-15
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "BonusModifierAbstract.h"
#include "NoeudRondelle.h"
#include "NoeudMaillet.h"
#include "Terrain.h"
#include "NodeBonus.h"
#ifdef WIN32
    #include "Partie.h"
#endif

////////////////////////////////////////////////////////////////////////
///
/// @fn bool BonusModifierAbstract::AttachToLastHittingMallet( NoeudAbstrait* pPuck )
///
/// utility functions, attachs the modifier to the last hitting mallet
///
/// @param[in] NoeudAbstrait * pPuck
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool BonusModifierAbstract::AttachToLastHittingMallet( NoeudRondelle* pPuck )
{
    if(pPuck)
    {
        auto maillet = pPuck->getLastHittingMallet();
        if(maillet)
        {
            // On tente d'appliquer le bonus sur le dernier maillet qui a frapper la rondelle
            mOwner = maillet;

            /// indicate we were able to attach the modifier
            return mOwner->AddModifier(this);
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool BonusModifierAbstract::AttachToOpposingMallet( NoeudRondelle* pPuck )
///
/// utility functions, attachs the modifier to the opposing mallet of the last hitting mallet
///
/// @param[in] NoeudRondelle * pPuck
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool BonusModifierAbstract::AttachToOpposingMallet( NoeudRondelle* pPuck )
{
    if(pPuck)
    {
        auto field = pPuck->getField();
        if(field)
        {
            auto lastHittingMallet = pPuck->getLastHittingMallet();
            auto leftMaillet = field->getLeftMallet();
            auto rightMaillet = field->getRightMallet();

            mOwner = lastHittingMallet == leftMaillet ? leftMaillet : rightMaillet;

            /// indicate we were able to attach the modifier
            return mOwner && mOwner->AddModifier(this);
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool BonusModifierAbstract::AttachToPuck( NoeudRondelle* pPuck )
///
/// utility functions, attachs the modifier to the puck
///
/// @param[in] NoeudRondelle * pPuck
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool BonusModifierAbstract::AttachToPuck( NoeudRondelle* pPuck )
{
    if(pPuck)
    {
        mOwner = pPuck;

        /// indicate we were able to attach the modifier
        return mOwner->AddModifier(this);
    }
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void BonusModifierAbstract::Tick( float temps )
///
/// Default tick behavior for time based modifiers
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void BonusModifierAbstract::Tick( float temps )
{
    #ifdef WIN32
    if(!IsFinished())
    {
        mTimeToLive -= temps;
        auto field = mCreator->getField();
        if(field)
        {
            auto game = field->GetGame();
            if(game)
            {
                mTimeToLive = 10 - (game->obtenirGameTime()->Elapsed_Time_sec() - mBeginTime);
            }
        }

        if(mTimeToLive < 0)
        {
            Revert();
            Complete();
        }
    }
    #endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void BonusModifierAbstract::Init( NodeBonus* creator )
///
/// Init function for factories
///
/// @param[in] NodeBonus * creator
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void BonusModifierAbstract::Init( NodeBonus* creator )
{
    #ifdef WIN32
    mCreator = creator;
    auto field = mCreator->getField();
    if(field)
    {
        auto game = field->GetGame();
        if(game)
        {
            mBeginTime = game->obtenirGameTime()->Elapsed_Time_sec();
            return;
        }
    }
    mBeginTime = clock()/CLOCKS_PER_SEC;
    #endif
}
