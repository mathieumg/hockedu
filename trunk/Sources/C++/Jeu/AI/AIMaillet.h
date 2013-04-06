//////////////////////////////////////////////////////////////////////////////
/// @file AIMaillet.h
/// @author Michael Ferris
/// @date 2012-04-02
/// @version 1.0
///
/// @addtogroup razergame RazerGame
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
	virtual ~AIMaillet();
	/// Évaluation de la stratégie à utiliser
	virtual void evaluerStrategie(NoeudMaillet* maillet);
	/// Évaluation de la direction
	Vecteur2 evaluerDirection(NoeudMaillet* maillet);
	/// Changement de strategie
	void changerStrat(typeStrat);

protected:
	/// La stratégie du maillet
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