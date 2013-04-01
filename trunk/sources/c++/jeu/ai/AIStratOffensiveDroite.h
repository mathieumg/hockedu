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
#include "aistrat.h"

///////////////////////////////////////////////////////////////////////////
/// @class AIStratOffensiveDroite
/// @brief Représente la stratégie d'un maillet en mode offensif droit.
///
/// @author Mathieu Parent
/// @date 2013-03-31
///////////////////////////////////////////////////////////////////////////
class AIStratOffensiveDroite : public AIStrat
{
public:
	/// Constructeur
	AIStratOffensiveDroite(const AIMaillet&);
	/// Destructeur
	virtual ~AIStratOffensiveDroite();
	/// Retourne la direction à prendre pour le maillet selon une stratégie offensive
	virtual Vecteur2 appliquerStrategie( NoeudMaillet* maillet );

/// Attributs
private:
	bool tirReussi_;


/// Accesseurs
public:
	virtual typeStrat obtenirType() { return OFFENSIVE_DROITE; }

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////