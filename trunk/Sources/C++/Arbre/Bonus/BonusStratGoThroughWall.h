///////////////////////////////////////////////////////////////////////////
/// @file BonusStratGoThroughWall.h
/// @author Michael Ferris
/// @date 2013-03-14
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once
#include "BonusStratAbstract.h"

///////////////////////////////////////////////////////////////////////////
/// @class BonusStratGoThroughWall
/// @brief Classe qui représente un bonus permettant au maillet de passer au travers des murs
///
/// @author Michael Ferris
/// @date 2013-03-14
///////////////////////////////////////////////////////////////////////////
class BonusStratGoThroughWall : public BonusStratAbstract
{
public:
    typedef BonusStratAbstract Super;
    virtual void Execute(class NoeudRondelle* pRondelle);
    virtual const std::string& get3DModelKey(){return Super::get3DModelKey();}
    virtual void Tick(float temps);
};