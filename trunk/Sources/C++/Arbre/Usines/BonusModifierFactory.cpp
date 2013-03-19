///////////////////////////////////////////////////////////////////////////////
/// @file BonusModifierFactory.cpp
/// @author Michael Ferris
/// @date 2013-03-14
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "BonusModifierFactory.h"
#include "BonusModifierGoThorughWall.h"
#include "BonusModifierBlockGoal.h"

DEFAULT_BONUS_FACTORY_IMPLEMENTATION(BonusModifierGoThroughWall);
DEFAULT_BONUS_FACTORY_IMPLEMENTATION(BonusModifierBlockGoal);


FactoryBonusModifier* FactoryBonusModifier::mFactories[NB_BONUS_TYPE] =
{
    new FactoryBonusModifierGoThroughWall(),
    new FactoryBonusModifierBlockGoal(),
};

////////////////////////////////////////////////////////////////////////
///
/// @fn void FactoryBonusModifier::ClearFactories()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FactoryBonusModifier::ClearFactories()
{
    for(int i=0; i<NB_BONUS_TYPE; ++i)
    {
        delete mFactories[i];
    }
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
