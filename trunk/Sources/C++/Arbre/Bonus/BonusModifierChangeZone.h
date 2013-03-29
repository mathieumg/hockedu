///////////////////////////////////////////////////////////////////////////
/// @file BonusModifierChangeZone.h
/// @author Michael Ferris
/// @date 2013-03-29
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once
#include "BonusModifierAbstract.h"
#include <vector>
#include "Vecteur.h"

class NoeudAbstrait;
///////////////////////////////////////////////////////////////////////////
/// @class BonusModifierChangeZone
/// @brief Classe qui représente un bonus permettant au maillet de changer de zone
///
/// @author Michael Ferris
/// @date 2013-03-29
///////////////////////////////////////////////////////////////////////////
class BonusModifierChangeZone : public BonusModifierAbstract
{
public:
    typedef BonusModifierAbstract Super;
    BonusModifierChangeZone();
    ~BonusModifierChangeZone();

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

    virtual BonusType getType()const{return BONUS_TYPE_CHANGE_ZONE;}
private:
    std::vector<class b2Fixture*> mFixtures;
};


////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////