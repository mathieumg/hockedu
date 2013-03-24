//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatAjoutPortail.h
/// @author Gabriel Couvrette
/// @date 2012-02-10
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "SourisEtatAbstrait.h"
#include "ArbreRenduINF2990.h"

////////////////////////////////////////////////////////////////////////////
/// @class SourisEtatEdition
/// @brief Classe de gestion du comportement dans le mode SourisEtatAjoutPortail
///
/// @author Gabriel Couvrette
/// @date 2012-01-21
////////////////////////////////////////////////////////////////////////////
class SourisEtatAjoutPortail :
	public SourisEtatAbstrait
{
public:
	/// Constructeur par paramètre
	SourisEtatAjoutPortail();
	/// Destructeur virtuel
	virtual ~SourisEtatAjoutPortail(void);

	/// Méthode qui cree des noeuds sur le parent en parametres
	void genererNoeud();

	/// Comportement lorsqu'un bouton de la souris est enfoncé
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris );
	/// Comportement lorsqu'un bouton de la souris est relâché
	virtual void sourisRelachee( EvenementSouris& evenementSouris );
	/// Comportement lorsque la souris est déplacée
	virtual void sourisDeplacee( EvenementSouris& evenementSouris );

	/// Retourne l'état courant de la souris
	virtual NomEtatSouris obtenirNomEtatSouris();
	

private : 
	/// Pointeur vers le portail en train d'être ajouté
	NoeudAbstrait* portailConcerne_;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


