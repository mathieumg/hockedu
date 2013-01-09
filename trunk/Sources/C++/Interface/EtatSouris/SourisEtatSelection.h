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
/// @brief Classe de gestion du comportement pour l'�tat S�lection.
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
	/// Indique s'il s'agit d'une selection rectangle ou non
	bool drag_;
	Vecteur2i rectangleElastiqueAncienCoin_;
	/// Conserve en m�moire la position de la souris depuis le dernier appel
	Vecteur2i positionPrecedente_;
	/// Conserve en m�moire la position de la souris lorsquelle a ete enfoncee
	Vecteur2i positionEnfoncee_;
	/// Conserver en m�moire si la touche ctrl est enfonc�e.
	bool ctrlEnfoncee_;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


