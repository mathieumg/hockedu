//////////////////////////////////////////////////////////////////////////////
/// @file AIStratOffensiveLigneDroite.h
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
/// @class AIStratOffensiveLigneDroite
/// @brief Repr�sente la strat�gie d'un maillet en mode offensif ligne droite.
///
/// @author Mathieu Parent
/// @date 2013-03-31
///////////////////////////////////////////////////////////////////////////
class AIStratOffensiveLigneDroite : public AIStrat
{
public:
	/// Constructeur
	AIStratOffensiveLigneDroite(const AIMaillet&);
	/// Destructeur
	virtual ~AIStratOffensiveLigneDroite();
	/// Retourne la direction � prendre pour le maillet selon une strat�gie offensive
	virtual Vecteur2 appliquerStrategie( NoeudMaillet* maillet, Vecteur2 pointVise = Vecteur2() );

/// Attributs
private:


/// Accesseurs
public:
	virtual typeStrat obtenirType() { return OFFENSIVE_LIGNE_DROITE; }

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////