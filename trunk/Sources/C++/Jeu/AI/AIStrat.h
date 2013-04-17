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
#include "GameTime.h"

class NoeudMaillet;
class AIMaillet;

enum typeStrat {DEFENSIVE, OFFENSIVE, OFFENSIVE_LIGNE_DROITE, OFFENSIVE_GAUCHE, OFFENSIVE_DROITE, NBSTRAT};


///////////////////////////////////////////////////////////////////////////
/// @class AIStrat
/// @brief Classe abstraite représentant une stratégie de joueur virtuelle.
///
/// @author Michael Ferris
/// @date 2012-04-02
///////////////////////////////////////////////////////////////////////////
class AIStrat
{
public:

    static bool TesterPushPuckOnTheOtherSide(class Partie* pPartie, NoeudMaillet* pMaillet);

	/// Constructeur
	AIStrat(const AIMaillet&);
	/// Destructeur virtuel
	virtual ~AIStrat();
	/// Méthode réimplémenté par les classes dérivés qui contient la stratégie selon le cas
	virtual Vecteur2 appliquerStrategie(NoeudMaillet* maillet)=0;

	inline void resetGameTime() {mGameTime.reset_Time();}

/// Attributs
protected:
	const AIMaillet& context_;
    GameTime mGameTime;

/// Accesseurs
public:
	virtual typeStrat obtenirType() = 0;


};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




