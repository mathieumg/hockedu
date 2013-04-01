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

///////////////////////////////////////////////////////////////////////////
/// @class AIStratOffensiveGauche
/// @brief Repr�sente la strat�gie d'un maillet en mode offensif gauche.
///
/// @author Mathieu Parent
/// @date 2013-03-31
///////////////////////////////////////////////////////////////////////////
class AIStratOffensiveGauche : public AIStrat
{
public:
	/// Constructeur
	AIStratOffensiveGauche(const AIMaillet&);
	/// Destructeur
	virtual ~AIStratOffensiveGauche();
	/// Retourne la direction � prendre pour le maillet selon une strat�gie offensive
	virtual Vecteur2 appliquerStrategie( NoeudMaillet* maillet );

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