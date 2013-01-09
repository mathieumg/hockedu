//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatTransformationRatation.h
/// @author Michael Ferris
/// @date 2012-01-21
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "SourisEtatAbstrait.h"
#include "INF2990TypeDef.h"

////////////////////////////////////////////////////////////////////////////
/// @class SourisEtatTransformationRotation
/// @brief Classe de gestion du comportement pour la rotation d'elements de l'arbre de rendu
///
/// @author Mathieu Parent
/// @date 2012-01-28
////////////////////////////////////////////////////////////////////////////
class SourisEtatTransformationRotation :
	public SourisEtatAbstrait
{
public:
	/// Constructeur par parametres
	SourisEtatTransformationRotation();
	/// Destructeur virtuel
	virtual ~SourisEtatTransformationRotation(void);

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

	/// Retourne le centre de rotation.
	Vecteur2 obtenirCentreRot() const;

private:
	/// Indique si un bouton de la souris a �t� enfonc�
	bool estEnfoncee_;
	/// Conserve en m�moire la position de la souris depuis le dernier appel
	Vecteur2i positionPrecedente_;
	/// Centre autour duquel la rotation a effectuer
	Vecteur2 centreRot_;
	/// Vecteur de noeuds s�lectionn�s
	ConteneurNoeuds noeudsSelectionnes_;
	/// Vecteur de roation inverse
	int* rotationInverse_;
	/// Indique si on g�re les collisions
	bool ignoreCollision_;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


