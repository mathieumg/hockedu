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
///         sur un noeud pour lui permettre de passer aux travers des murs
///
/// @author Michael Ferris
/// @date 2013-03-14
///////////////////////////////////////////////////////////////////////////
class BonusModifierGoThroughWall : public BonusModifierAbstract
{
public:
    BonusModifierGoThroughWall();
    virtual bool Attach(NoeudAbstrait* pNode);
    virtual bool Apply();
    virtual bool Revert();
    virtual void Tick(float temps);
private:
    float mTimeToLive;
    std::vector<class b2Fixture*> mFixtures;
};