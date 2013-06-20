///////////////////////////////////////////////////////////////////////////////
/// @file BonusModifierSpawnPuck.cpp
/// @author Michael Ferris
/// @date 2013-03-29
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "BonusModifierSpawnPuck.h"

// ne fait pas de sens d'utiliser un modifier si on ne joue pas
#if BOX2D_PLAY  
#include <Box2D/Box2D.h>
#endif
#if WIN32
#include "SoundFMOD.h"
#endif
#include "NoeudAbstrait.h"
#include "Utilitaire.h"
#include "NodeBonus.h"
#include "Terrain.h"
#include "RazerGameTree.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  BonusModifierSpawnPuck::BonusModifierSpawnPuck()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
BonusModifierSpawnPuck::BonusModifierSpawnPuck():
    BonusModifierAbstract(0)
{
}



////////////////////////////////////////////////////////////////////////
///
/// @fn  BonusModifierSpawnPuck::~BonusModifierSpawnPuck()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
BonusModifierSpawnPuck::~BonusModifierSpawnPuck()
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool BonusModifierSpawnPuck::Attach( NoeudAbstrait* pPuck )
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
bool BonusModifierSpawnPuck::Attach( NoeudRondelle* pPuck )
{
    if(mCreator)
    {
        
#if WIN32 
        SoundFMOD::obtenirInstance()->playEffect(BONUS_FAN_IN_EFFECT); 
#endif

        mOwner = mCreator;
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool BonusModifierSpawnPuck::Apply()
///
/// Applies the real effect on the node
/// returns false if the bonus finished the execution (no time to live)
/// in case it returns false, complete its execution
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool BonusModifierSpawnPuck::Apply()
{
    if(mOwner)
    {
        auto field = mOwner->getField();
        auto tree = field->getLogicTree();
        if(tree)
        {
            auto mNewNode = tree->creerNoeud(RazerGameUtilities::NOM_RONDELLE);
            if(mNewNode)
            {
                mNewNode->setPosition(mOwner->getPosition());
                field->addNode(mNewNode);
            }
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool BonusModifierSpawnPuck::Revert()
///
/// Reverts the effect on the node,
/// return unsed for now
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool BonusModifierSpawnPuck::Revert()
{
    return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void BonusModifierSpawnPuck::render()
///
/// Render specific content for this modifier
/// translation to the node will already be applied
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void BonusModifierSpawnPuck::render() const
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void BonusModifierSpawnPuck::Tick( float temps )
///
/// Tick the modifier
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void BonusModifierSpawnPuck::Tick( float temps )
{
    BonusModifierAbstract::Tick(temps);
}


////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////