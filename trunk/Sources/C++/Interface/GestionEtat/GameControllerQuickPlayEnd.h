//////////////////////////////////////////////////////////////////////////////
/// @file GameControllerQuickPlayEnd.h
/// @author Vincent Lemire, Michael Ferris
/// @date 2012-04-13
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////


#pragma once
#include "GameControllerAbstract.h"


////////////////////////////////////////////////////////////////////////////
/// @class GameControllerQuickPlayEnd
/// @brief Classe abstraite dont les héritières gereront les états selon le
/// patron state.
///
/// @author Vincent Lemire, Michael Ferris
/// @date 2012-04-13
////////////////////////////////////////////////////////////////////////////
class GameControllerQuickPlayEnd :
	public GameControllerAbstract
{
public:
	/// Constructeur
	GameControllerQuickPlayEnd();
	/// Destructeur
	virtual ~GameControllerQuickPlayEnd(void);

	/// Comportement pour une touche enfoncée
	virtual void toucheEnfoncee( EvenementClavier& evenementClavier );

	/// Comportement pour une touche relachée
	virtual void toucheRelachee( EvenementClavier& evenementClavier );

	/// Comportement pour un bouton de souris enfoncé
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris );

	/// Comportement pour un bouton de souris relâché
	virtual void sourisRelachee( EvenementSouris& evenementSouris );

	/// Comportement lorsque la souris est déplacée
	virtual void sourisDeplacee( EvenementSouris& evenementSouris );

	/// Comportement lorsqu'on bouge la roulette de la souris
	virtual void rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris );

	/// Comportement de la methode animer 
	virtual void animer( const float& temps );
	/// Permet d'effectuer l'affichage specifique a l'etat
	virtual void afficher();

private:
    int mIdAnimMallet,mIdAnimCam;
};

