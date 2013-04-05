//////////////////////////////////////////////////////////////////////////////
/// @file AIMailletRenforcement.h
/// @author Mathieu Parent
/// @date 2013-03-31
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "AIMaillet.h"


///////////////////////////////////////////////////////////////////////////
/// @class AIMailletRenforcement
/// @brief Maillet d'un joueur virtuel par renforcement.
///
/// @author Mathieu Parent 
/// @date 2013-03-31
///////////////////////////////////////////////////////////////////////////
class AIMailletRenforcement : public AIMaillet
{
public:
	/// Constructeur
	AIMailletRenforcement(const JoueurVirtuel&);
	/// Destructeur
	virtual ~AIMailletRenforcement();
	/// Évaluation de la stratégie à utiliser
	virtual void evaluerStrategie(NoeudMaillet* maillet);
	/// Accesseur de lignePrevue
	std::pair<Vecteur2,Vecteur2> getLignePrevue() const { return lignePrevue; }
	void setLignePrevue(std::pair<Vecteur2,Vecteur2> val) { lignePrevue = val; }

private:
	/// Ligne de passage prévu de la rondelle, le premier vecteur2 est la position initiale de la rondelle, 
	/// et le deuxième vecteur2 est la position prévue de la rondelle sur la ligne de "calcul"
	std::pair <Vecteur2,Vecteur2> lignePrevue;  
	
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////