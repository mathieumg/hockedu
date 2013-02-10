//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatOrbit.h
/// @author Mathieu Parent
/// @date 2012-04-11
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SourisEtatAbstrait.h"

////////////////////////////////////////////////////////////////////////////
/// @class SourisEtatOrbit
/// @brief Classe de gestion du comportement dans le mode SourisEtatOrbit
///
/// @author Mathieu Parent
/// @date 2012-04-11
////////////////////////////////////////////////////////////////////////////
class SourisEtatOrbit:
	public SourisEtatAbstrait
{
public:
	/// Constructeur par parametres
	SourisEtatOrbit();
	/// Destructeur virtuel
	virtual ~SourisEtatOrbit(void);

	/// Comportement lorsqu'un bouton de la souris est enfoncé
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris );
	/// Comportement lorsqu'un bouton de la souris est relâché
	virtual void sourisRelachee( EvenementSouris& evenementSouris );
	/// Comportement lorsque la souris est déplacée
	virtual void sourisDeplacee( EvenementSouris& evenementSouris );

	/// Retourne l'état courant de la souris
	virtual NomEtatSouris obtenirNomEtatSouris();

private:
	/// Indique si les boutons de la souris sont enfoncés ou pas
	bool estEnfoncee_[NB_BOUTON_SOURIS];
	/// Conserve en mémoire la position de la souris depuis le dernier appel
	Vecteur2i dernierePosition_[NB_BOUTON_SOURIS];
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


