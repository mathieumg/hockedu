///////////////////////////////////////////////////////////////////////////
/// @file BonusModifierAbstract.h
/// @author Michael Ferris
/// @date 2013-03-14
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once

#include "Enum_Declarations.h"


class NoeudAbstrait;
class NoeudRondelle;
class NodeBonus;
///////////////////////////////////////////////////////////////////////////
/// @class BonusModifierAbstract
/// @brief Classe qui représente l'interface de base des bonus qui s'attachent
///         aux noeuds pour les modifier
///
/// @author Michael Ferris
/// @date 2013-03-14
///////////////////////////////////////////////////////////////////////////
class BonusModifierAbstract
{
public:
    // Default constructor
    BonusModifierAbstract(float timeToLive):mIsFinished(false),mOwner(0),mTimeToLive(timeToLive),mCreator(0)
    {
    }

    virtual ~BonusModifierAbstract(){}

    


    /// Attach a modifier on a node, receiving the puck as entry point,
    /// but from it, the modifier can apply itself on anything in the map
    ///
    /// Don't forget to set the owner!
    ///
    /// returns true if the modifier is attached on a node
    /// returns false otherwise
    /// note, returning false doesn't mean the bonus was not applied or refreshed
    virtual bool Attach(NoeudRondelle* pNode) = 0;

    /// Applies the real effect on the node
    /// returns false if the bonus finished the execution (no time to live)
    /// in case it returns false, complete its execution
    virtual bool Apply() = 0;
    /// Reverts the effect on the node,
    /// return unsed for now
    virtual bool Revert() = 0;

    /// Indicate if the modifier has finished the execution
    virtual bool IsFinished(){return mIsFinished;}

    /// Default tick behavior for time based modifiers
    virtual void Tick(float temps);

    /// Render specific content for this modifier
    /// translation to the node will already be applied
    virtual void render() const {}

    /// Completes the modifiers execution 
    void Complete(){mIsFinished = true;}

    virtual BonusType getType()const = 0;
    /// Accessors of mTimeToLive
    inline float getTimeToLive() const { return mTimeToLive; }

    /// Init function for factories
    void Init(NodeBonus* creator);


    inline NoeudAbstrait* getOwner() const { return mOwner; }

protected:
    /// utility functions, attachs the modifier to the last hitting mallet
    bool AttachToLastHittingMallet(NoeudRondelle* pPuck);
    /// utility functions, attachs the modifier to the opposing mallet of the last hitting mallet
    bool AttachToOpposingMallet(NoeudRondelle* pPuck);
    /// utility functions, attachs the modifier to the puck
    bool AttachToPuck(NoeudRondelle* pPuck);

    NoeudAbstrait* mOwner;
    
    float mTimeToLive;
    NodeBonus* mCreator;
private:
    bool mIsFinished;
};
