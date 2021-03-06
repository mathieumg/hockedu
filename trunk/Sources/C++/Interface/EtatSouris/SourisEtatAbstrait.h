//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatAbsrait.h
/// @author Michael Ferris
/// @date 2012-01-21
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "GameControllerAbstract.h"
#include "NoeudAbstrait.h"

////////////////////////////////////////////////////////////////////////////
/// @class SourisEtatAbstrait
/// @brief Interface de l'�tat courant de la souris pour la gestion de son comportement dans le jeu
///		   Classe abstraite.
/// @author Michael Ferris
/// @date 2012-01-21
////////////////////////////////////////////////////////////////////////////
class SourisEtatAbstrait
{
public:
	/// Constructeur par d�faut
	SourisEtatAbstrait( );
	/// Destructeur virtuelle pure
	virtual ~SourisEtatAbstrait(void) = 0;

	/// Comportement lorsqu'une touche du clavier est enfonc�e.
	virtual void toucheEnfoncee( EvenementClavier& evenementClavier ) {}
	/// Comportement lorsqu'une touche du clavier est rel�ch�e.
	virtual void toucheRelachee( EvenementClavier& evenementClavier ) {}
	/// Comportement lorsqu'un bouton de la souris est enfonc�
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris ) = 0 ;
	/// Comportement lorsqu'un bouton de la souris est relach�.
	virtual void sourisRelachee( EvenementSouris& evenementSouris ) = 0;
	/// Comportement lorsque la souris est d�plac�e.
	virtual void sourisDeplacee( EvenementSouris& evenementSouris ) = 0;
    virtual void doubleClickEvent( EvenementSouris& evenementSouris )
    {
        // default behavior is to simply send a second release event
        sourisRelachee(evenementSouris);
    }
    virtual bool OverrideAnimate(float temps){return false;}
    virtual bool OverrideRender(){return false;}
	/// Retourne l'�tat courant
	virtual NomEtatSouris obtenirNomEtatSouris() = 0;

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


