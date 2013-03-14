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
    BonusModifierAbstract():mIsFinished(false),mOwner(0)
    {

    }
    virtual bool Attach(NoeudAbstrait* pNode) = 0;
    virtual bool Apply() = 0;
    virtual bool Revert() = 0;
    virtual bool IsFinished(){return mIsFinished;}
    virtual void Tick(float temps) {}
protected:
    void Complete(){mIsFinished = true;}

    NoeudAbstrait* mOwner;
private:
    bool mIsFinished;
};