///////////////////////////////////////////////////////////////////////////
/// @file BonusModifierGoThroughWall.h
/// @author Michael Ferris
/// @date 2013-03-14
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once
#include "BonusModifierAbstract.h"
#include <vector>

class NoeudAbstrait;
///////////////////////////////////////////////////////////////////////////
/// @class BonusModifierGoThroughWall
/// @brief Classe qui représente un bonus appliquant la modification
///         sur la rondelle pour lui permettre de passer aux travers des murs
///
/// @author Michael Ferris
/// @date 2013-03-14
///////////////////////////////////////////////////////////////////////////
class BonusModifierGoThroughWall : public BonusModifierAbstract
{
public:
    BonusModifierGoThroughWall();

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

    virtual BonusType getType()const{return BONUS_TYPE_GO_THROUGH_WALL;}
private:
    std::vector<class b2Fixture*> mFixtures;
};

///////////////////////////////////////////////////////////////////////////
/// @class BonusModifierGoThroughWallMallet
/// @brief Classe qui représente un bonus appliquant la modification
///         sur le maillet pour lui permettre de passer aux travers des murs
///
/// @author Vincent Lemire
/// @date 2013-04-06
///////////////////////////////////////////////////////////////////////////
class BonusModifierGoThroughWallMallet : public BonusModifierGoThroughWall
{
public:
    BonusModifierGoThroughWallMallet(){}

    virtual bool Attach(NoeudRondelle* pPuck);
    virtual BonusType getType()const{return BONUS_TYPE_GO_THROUGH_WALL_MALLET;}
};


////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////