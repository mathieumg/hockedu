//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatAbsrait.h
/// @author Michael Ferris
/// @date 2012-01-21
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Vecteur.h"
#include "GestionnaireEtatAbstrait.h"


////////////////////////////////////////////////////////////////////////////
/// @class SourisEtatAbstrait
/// @brief Interface de l'état courant de la souris pour la gestion de son comportement dans le jeu
///		   Classe abstraite.
/// @author Michael Ferris
/// @date 2012-01-21
////////////////////////////////////////////////////////////////////////////
class SourisEtatAbstrait
{
public:
	/// Constructeur par défaut
	SourisEtatAbstrait( );
	/// Destructeur virtuelle pure
	virtual ~SourisEtatAbstrait(void) = 0;

	/// Comportement lorsqu'une touche du clavier est enfoncée.
	virtual void toucheEnfoncee( EvenementClavier& evenementClavier ) {}
	/// Comportement lorsqu'une touche du clavier est relâchée.
	virtual void toucheRelachee( EvenementClavier& evenementClavier ) {}
	/// Comportement lorsqu'un bouton de la souris est enfoncé
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris ) = 0 ;
	/// Comportement lorsqu'un bouton de la souris est relaché.
	virtual void sourisRelachee( EvenementSouris& evenementSouris ) = 0;
	/// Comportement lorsque la souris est déplacée.
	virtual void sourisDeplacee( EvenementSouris& evenementSouris ) = 0;

	/// Retourne l'état courant
	virtual NomEtatSouris obtenirNomEtatSouris() = 0;

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


