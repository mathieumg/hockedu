//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatSelection.h
/// @author Mathieu Parent
/// @date 2012-02-06
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "SourisEtatAbstrait.h"



////////////////////////////////////////////////////////////////////////////
/// @class SourisEtatSelection
/// @brief Classe de gestion du comportement pour l'état Sélection.
///
/// @author Mathieu Parent
/// @date 2012-02-06
////////////////////////////////////////////////////////////////////////////
class SourisEtatSelection :
	public SourisEtatAbstrait
{
public:
	/// Constructeur par parametres
	SourisEtatSelection();
	/// Destructeur virtuel
	virtual ~SourisEtatSelection(void);

	/// Comportement lorsqu'une touche du clavier est enfoncée.
	virtual void toucheEnfoncee( EvenementClavier& evenementClavier );
	/// Comportement lorsqu'une touche du clavier est relâchée.
	virtual void toucheRelachee( EvenementClavier& evenementClavier );
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
	/// Indique s'il s'agit d'une selection rectangle ou non
	bool drag_;
	Vecteur2i rectangleElastiqueAncienCoin_;
	/// Conserve en mémoire la position de la souris depuis le dernier appel
	Vecteur2i positionPrecedente_;
	/// Conserve en mémoire la position de la souris lorsquelle a ete enfoncee
	Vecteur2i positionEnfoncee_;
	/// Conserver en mémoire si la touche ctrl est enfoncée.
	bool ctrlEnfoncee_;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


