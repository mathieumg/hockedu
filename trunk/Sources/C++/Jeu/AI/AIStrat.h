//////////////////////////////////////////////////////////////////////////////
/// @file AIStrat.h
/// @author Michael Ferris
/// @date 2012-04-02
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Vecteur.h"

class NoeudMaillet;
class AIMaillet;

enum typeStrat {DEFENSIVE,OFFENSIVE,NBSTRAT};

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
	virtual Vecteur2 appliquerStrategie(NoeudMaillet* maillet)=0;

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




