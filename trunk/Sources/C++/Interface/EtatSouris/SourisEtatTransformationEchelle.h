//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatTransformationEchelle.h
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
/// @class SourisEtatTransformationEchelle
/// @brief Classe de gestion du comportement pour la mise a l'echelle des elements de l'arbre de rendu
///
/// @author Mathieu Parent
/// @date 2012-01-28
////////////////////////////////////////////////////////////////////////////
class SourisEtatTransformationEchelle :
	public SourisEtatAbstrait
{
public:
	/// Constructeur par parametres
	SourisEtatTransformationEchelle();
	/// Destructeur virtuel
	virtual ~SourisEtatTransformationEchelle(void);

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
	/// Contient les noeuds s�lectionn�s
	ConteneurNoeuds noeudSelectionnes_;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


