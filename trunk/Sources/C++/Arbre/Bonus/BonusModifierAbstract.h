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

class NoeudAbstrait;
class NoeudRondelle;
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
    BonusModifierAbstract():mIsFinished(false),mOwner(0),mTimeToLive(10.f)
    {
    }

    virtual ~BonusModifierAbstract(){}

    /// Attach a modifier on a node, receiving the puck as entry point,
    /// but from it, the modifier can apply itself on anything in the map
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

    /// Completes the modifiers execution 
    void Complete(){mIsFinished = true;}
protected:

    /// utility functions, attachs the modifier to the last hitting mallet
    bool AttachToLastHittingMallet(NoeudRondelle* pPuck);
    /// utility functions, attachs the modifier to the opposing mallet of the last hitting mallet
    bool AttachToOpposingMallet(NoeudRondelle* pPuck);
    /// utility functions, attachs the modifier to the puck
    bool AttachToPuck(NoeudRondelle* pPuck);

    NoeudAbstrait* mOwner;
    float mTimeToLive;
private:
    bool mIsFinished;
};
