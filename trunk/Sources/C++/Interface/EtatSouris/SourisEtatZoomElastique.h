//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatZoomElastique.h
/// @author Michael Ferris
/// @date 2012-01-21
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SourisEtatAbstrait.h"

////////////////////////////////////////////////////////////////////////////
/// @class SourisEtatZoomElastique
/// @brief Classe de gestion du comportement du Zoom Elastique
///
/// @author Michael Ferris
/// @date 2012-01-21
////////////////////////////////////////////////////////////////////////////
class SourisEtatZoomElastique:
	public SourisEtatAbstrait
{
public:
	/// Constructeur par parametres
	SourisEtatZoomElastique();
	/// Destructeur virtuel
	virtual ~SourisEtatZoomElastique(void);
	/// Comportement lorsqu'une touche du clavier est enfonc�e.
	virtual void toucheEnfoncee( EvenementClavier& evenementClavier );
	/// Comportement lorsqu'une touche du clavier est rel�ch�e.
	virtual void toucheRelachee( EvenementClavier& evenementClavier );
	/// Comportement lorsqu'un bouton de la souris est enfonc�.
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris );
	/// Comportement lorsqu'un bouton de la souris est relach�.
	virtual void sourisRelachee( EvenementSouris& evenementSouris );
	/// Comportement lorsque la souris est d�plac�e.
	virtual void sourisDeplacee( EvenementSouris& evenementSouris );

	/// Retourne l'�tat courant
	virtual NomEtatSouris obtenirNomEtatSouris();

private:
	/// Indique si un bouton de la souris a �t� enfonc�
	bool estEnfoncee_;
	/// Conserve en m�moire quel bouton a �t� enfonc�
	BoutonSouris boutonEnfonce_;
	/// Conserve en m�moire la position de la souris lorsqu'un bouton a �t� enfonc�
	Vecteur2i positionDepart_;
	/// Conserver en m�moire si la touche ctrl est enfonc�e.
	bool ctrlEnfoncee_;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


