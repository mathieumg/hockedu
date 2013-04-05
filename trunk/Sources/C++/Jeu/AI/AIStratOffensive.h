//////////////////////////////////////////////////////////////////////////////
/// @file AIStratOffensive.h
/// @author Michael Ferris
/// @date 2012-04-02
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "aistrat.h"

///////////////////////////////////////////////////////////////////////////
/// @class AIStratOffensive
/// @brief Repr�sente la strat�gie d'un maillet en mode offensif.
///
/// @author Michael Ferris
/// @date 2012-04-02
///////////////////////////////////////////////////////////////////////////
class AIStratOffensive : public AIStrat
{
public:
	/// Constructeur
	AIStratOffensive(const AIMaillet&);
	/// Destructeur
	virtual ~AIStratOffensive();
	/// Retourne la direction � prendre pour le maillet selon une strat�gie offensive
	virtual Vecteur2 appliquerStrategie( NoeudMaillet* maillet , Vecteur2 pointVise = Vecteur2() );

/// Attributs
private:
	bool tirReussi_;


/// Accesseurs
public:
	virtual typeStrat obtenirType() { return OFFENSIVE; }

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////