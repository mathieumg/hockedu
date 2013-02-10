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

	/// Comportement lorsqu'un bouton de la souris est enfoncé.
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris );
	/// Comportement lorsqu'un bouton de la souris est relaché.
	virtual void sourisRelachee( EvenementSouris& evenementSouris );
	/// Comportement lorsque la souris est déplacée.
	virtual void sourisDeplacee( EvenementSouris& evenementSouris );

	/// Retourne l'état courant
	virtual NomEtatSouris obtenirNomEtatSouris();

private:
	/// Indique si un bouton de la souris a été enfoncé
	bool estEnfoncee_;
	/// Conserve en mémoire la position de la souris depuis le dernier appel
	Vecteur2i positionPrecedente_;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


