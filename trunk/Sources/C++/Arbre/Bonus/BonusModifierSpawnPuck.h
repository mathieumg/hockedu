///////////////////////////////////////////////////////////////////////////
/// @file BonusModifierSpawnPuck.h
/// @author Michael Ferris
/// @date 2013-03-29
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once
#include "BonusModifierAbstract.h"

///////////////////////////////////////////////////////////////////////////
/// @class BonusModifierSpawnPuck
/// @brief Classe qui représente un bonus qui fait apparaitre une nouvelle rondelle
///
/// @author Michael Ferris
/// @date 2013-03-29
///////////////////////////////////////////////////////////////////////////
class BonusModifierSpawnPuck : public BonusModifierAbstract
{
public:
    BonusModifierSpawnPuck();
    ~BonusModifierSpawnPuck();

    /// Attach a modifier on a node, receiving the puck as entry point,
    /// but from it, the modifier can apply itself on anything in the map
    ///
    /// returns true if the modifier is attached on a node
    /// returns false otherwise
    /// note, returning false doesn't mean the bonus was not applied or refreshed
    virtual bool Attach(NoeudRondelle* pPuck);

    /// Applies the real effect on the node
    /// returns false if the bonus finished the execution (no time to live)
    /// in case it returns false, complete its execution
    virtual bool Apply();

    /// Reverts the effect on the node,
    /// return unsed for now
    virtual bool Revert();

    /// Render specific content for this modifier
    /// translation to the node will already be applied
    virtual void render() const;

    /// Tick the modifier
    virtual void Tick(float temps);

    virtual BonusType getType()const{return BONUS_TYPE_SPAWN_PUCK;}
};


////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////