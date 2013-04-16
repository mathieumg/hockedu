//////////////////////////////////////////////////////////////////////////////
/// @file AIStratOffensiveDroite.h
/// @author Mathieu Parent
/// @date 2013-03-31
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "AIStratOffensiveRenforcement.h"

///////////////////////////////////////////////////////////////////////////
/// @class AIStratOffensiveDroite
/// @brief Représente la stratégie d'un maillet en mode offensif droit.
///
/// @author Mathieu Parent
/// @date 2013-03-31
///////////////////////////////////////////////////////////////////////////
class AIStratOffensiveDroite : public AIStratOffensiveRenforcement
{
public:
	/// Constructeur
    AIStratOffensiveDroite(const AIMaillet&);
	/// Destructeur
	virtual ~AIStratOffensiveDroite();
/// Attributs
private:
	bool tirReussi_;


/// Accesseurs
public:
	virtual typeStrat obtenirType() { return OFFENSIVE_DROITE; }

    virtual void calculateTargetPos();

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////