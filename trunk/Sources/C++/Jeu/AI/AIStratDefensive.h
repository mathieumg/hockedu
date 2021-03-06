//////////////////////////////////////////////////////////////////////////////
/// @file AIStratDefensive.h
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
/// @class AIStratDefensive
/// @brief Repr�sente la strat�gie d'un joueur virtuel en mode d�fense.
///
/// @author Michael Ferris
/// @date 2012-04-02
///////////////////////////////////////////////////////////////////////////
class AIStratDefensive : public AIStrat
{
public:
	/// Constructeur
	AIStratDefensive(const AIMaillet&);

	/// Destructeur
	virtual ~AIStratDefensive();

	/// Retourne la direction � prendre pour le maillet selon une strat�gie defensive
	virtual Vecteur2 appliquerStrategie( NoeudMaillet* maillet);

/// Accesseurs
public:
	virtual typeStrat obtenirType() { return DEFENSIVE; }

private:
	float mPosToDefend;
	float mErreurY;
    
};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////