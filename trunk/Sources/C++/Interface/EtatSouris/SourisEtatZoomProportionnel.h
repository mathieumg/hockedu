//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatZoomProportionnel.h
/// @author Michael Ferris
/// @date 2012-01-21
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "SourisEtatAbstrait.h"

class GestionnaireEvenementsTest;

////////////////////////////////////////////////////////////////////////////
/// @class SourisEtatZoomProportionnel
/// @brief Classe de gestion du comportement pour le Zoom Proportionnel
///
/// @author Michael Ferris
/// @date 2012-01-21
////////////////////////////////////////////////////////////////////////////
class SourisEtatZoomProportionnel :
	public SourisEtatAbstrait
{
public:

	/// Pour les tests unitaires
	friend GestionnaireEvenementsTest;

	/// Constructeur par parametres
	SourisEtatZoomProportionnel();
	/// Destructeur virtuel
	virtual ~SourisEtatZoomProportionnel(void);

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
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


