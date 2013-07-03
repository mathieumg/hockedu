///////////////////////////////////////////////////////////////////////////
/// @file BonusModifierBlockGoal.h
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
#include "Vecteur.h"

class NoeudAbstrait;
///////////////////////////////////////////////////////////////////////////
/// @class BonusModifierBlockGoal
/// @brief Classe qui représente un bonus appliquant la modification
///         sur un but pour empecher la rondelle de passer
///
/// @author Michael Ferris
/// @date 2013-03-14
///////////////////////////////////////////////////////////////////////////
class BonusModifierBlockGoal : public BonusModifierAbstract
{
public:
    BonusModifierBlockGoal();

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

    virtual BonusType getType()const{return BONUS_TYPE_BLOCK_GOAL;}
private:
    /// maximum value possible to render the goalie,
    /// taken from the goal height
    class NoeudBut* mProtectedGoal;
    Vecteur3 mPosition;
    float mAngle; // en radian


    std::vector<class b2Fixture*> mFixtures;
    class b2Body* mPhysicBody;

    void getGoalPosition(Vecteur3& pos, float& angle)const;

};