//////////////////////////////////////////////////////////////////////////////
/// @file AIMaillet.h
/// @author Michael Ferris
/// @date 2012-04-02
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Vecteur.h"
#include "AIStrat.h"

class NoeudMaillet;
class JoueurVirtuel;

///////////////////////////////////////////////////////////////////////////
/// @class AIMaillet
/// @brief Maillet d'un joueur virtuel.
///
/// @author Michael Ferris
/// @date 2012-02-17
///////////////////////////////////////////////////////////////////////////
class AIMaillet
{
public:
	/// Constructeur
	AIMaillet(const JoueurVirtuel&);
	/// Destructeur
	~AIMaillet();
	/// �valuation de la strat�gie � utiliser
	void evaluerStrategie(NoeudMaillet* maillet);
	/// �valuation de la direction
	Vecteur2 evaluerDirection(NoeudMaillet* maillet);
	/// Changement de strategie
	void changerStrat(typeStrat);

private:
	/// La strat�gie du maillet
	AIStrat* strategie_;
	const JoueurVirtuel& jv_;

/// Accesseurs
public:
	/// Accesseur de jv_
	const JoueurVirtuel& obtenirJv() const { return jv_; }

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////