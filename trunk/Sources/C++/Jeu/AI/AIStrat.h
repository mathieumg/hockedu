//////////////////////////////////////////////////////////////////////////////
/// @file AIStrat.h
/// @author Michael Ferris
/// @date 2012-04-02
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Vecteur.h"

class NoeudMaillet;
class AIMaillet;

enum typeStrat {DEFENSIVE, OFFENSIVE, OFFENSIVE_LIGNE_DROITE, OFFENSIVE_GAUCHE, OFFENSIVE_DROITE, NBSTRAT};

///////////////////////////////////////////////////////////////////////////
/// @class AIStrat
/// @brief Classe abstraite repr�sentant une strat�gie de joueur virtuelle.
///
/// @author Michael Ferris
/// @date 2012-04-02
///////////////////////////////////////////////////////////////////////////
class AIStrat
{
public:
	/// Constructeur
	AIStrat(const AIMaillet&);
	/// Destructeur virtuel
	virtual ~AIStrat();
	/// M�thode r�impl�ment� par les classes d�riv�s qui contient la strat�gie selon le cas
	virtual Vecteur2 appliquerStrategie(NoeudMaillet* maillet, Vecteur2 pointVise = Vecteur2())=0;

/// Attributs
protected:
	const AIMaillet& context_;

/// Accesseurs
public:
	virtual typeStrat obtenirType() = 0;

};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




