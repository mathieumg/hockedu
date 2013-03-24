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

private:
	/// Indique si un bouton de la souris a �t� enfonc�
	bool estEnfoncee_;
	/// Conserve en m�moire la position de la souris depuis le dernier appel
	Vecteur2i positionPrecedente_;
	/// Attribut indiquant si les d�placement ignore les collisions
	bool ignoreCollision_;
	/// Vecteur de noeuds s�lectionn�s
	ConteneurNoeuds noeudsSelectionnes_;
	/// Contient l'inverse du d�placement effectu� (en cas d'annulation)
	Vecteur2* deplacementInverse_;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


