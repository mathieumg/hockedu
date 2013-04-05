//////////////////////////////////////////////////////////////////////////////
/// @file AIStratOffensiveGauche.h
/// @author Mathieu Parent
/// @date 2013-03-31
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "aistrat.h"
#include "AIStratOffensiveRenforcement.h"

///////////////////////////////////////////////////////////////////////////
/// @class AIStratOffensiveGauche
/// @brief Représente la stratégie d'un maillet en mode offensif gauche.
///
/// @author Mathieu Parent
/// @date 2013-03-31
///////////////////////////////////////////////////////////////////////////
class AIStratOffensiveGauche : public AIStratOffensiveRenforcement
{
public:
	/// Constructeur
	AIStratOffensiveGauche(const AIMaillet&);
	/// Destructeur
	virtual ~AIStratOffensiveGauche();
/// Attributs
private:
	bool tirReussi_;


/// Accesseurs
public:
	virtual typeStrat obtenirType() { return OFFENSIVE_GAUCHE; }

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////