///////////////////////////////////////////////////////////////////////////
/// @file BonusStratAbstract.h
/// @author Michael Ferris
/// @date 2013-03-14
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once
#include <string>


///////////////////////////////////////////////////////////////////////////
/// @class BonusStratAbstract
/// @brief Classe qui représente l'interface de base des bonus concret qu'un
///         joueur peut obtenir
///
/// @author Michael Ferris
/// @date 2013-03-14
///////////////////////////////////////////////////////////////////////////
class BonusStratAbstract
{
public:
    virtual void Execute(class NoeudRondelle* pRondelle) = 0;

    // by default we draw the same model as an empty model
    virtual const std::string& get3DModelKey();
    virtual void Tick(float temps) {}
};

