//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatTransformationDeplacement.h
/// @author Michael Ferris
/// @date 2012-01-21
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "SourisEtatAbstrait.h"
#include "RazerGameTypeDef.h"

////////////////////////////////////////////////////////////////////////////
/// @class SourisEtatTransformationDeplacement
/// @brief Classe de gestion du comportement pour le d�placement des �l�ments � l'aide de la souris
///
/// @author Mathieu Parent
/// @date 2012-01-28
////////////////////////////////////////////////////////////////////////////
class SourisEtatTransformationDeplacement :
	public SourisEtatAbstrait
{
public:
	/// Constructeur par parametres
	SourisEtatTransformationDeplacement();
	/// Destructeur virtuel
	virtual ~SourisEtatTransformationDeplacement(void);

	/// Comportement lorsqu'une touche du clavier est enfonc�.
	virtual void toucheEnfoncee( EvenementClavier& evenementClavier );
	/// Comportement lorsqu'une touche du clavier est enfonc�.
	virtual void toucheRelachee( EvenementClavier& evenementClavier );
	/// Comportement lorsqu'un bouton de la souris est enfonc�.
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris );
	/// Comportement lorsqu'un bouton de la souris est relach�.
	virtual void sourisRelachee( EvenementSouris& evenementSouris );
	/// Comportement lorsque la souris est d�plac�e.
	virtual void sourisDeplacee( EvenementSouris& evenementSouris );

	/// Retourne l'�tat courant
	virtual NomEtatSouris obtenirNomEtatSouris();
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


