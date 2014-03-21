//////////////////////////////////////////////////////////////////////////////
/// @file GameControllerSimulation.h
/// @author Michael Ferris
/// @date 2013-03-03
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef GESTIONNAIREETATMODESIMULATION_H
#define GESTIONNAIREETATMODESIMULATION_H

#include "GameControllerAbstract.h"
#include <vector>

typedef int IDPartie;
typedef std::vector<IDPartie> GamesContainer;

////////////////////////////////////////////////////////////////////////////
/// @class GameControllerSimulation
/// @brief Classe qui gère les événement l'affichage 
///         et la mise a jours lorsqu'en mode simulation
///
/// @author Michael Ferris
/// @date 2013-03-03
////////////////////////////////////////////////////////////////////////////
class GameControllerSimulation : public GameControllerAbstract
{
public:
	/// Constructeur par paramètre.
	GameControllerSimulation();
	/// Destructeur virtuel
	virtual ~GameControllerSimulation();

	/// Comportement pour une touche enfoncée
	virtual void toucheEnfoncee(EvenementClavier& evenementClavier);
	/// Comportement pour une touche relachée
	virtual void toucheRelachee(EvenementClavier& evenementClavier);
	/// Comportement pour un bouton de souris enfoncé
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris);
	/// Comportement pour un bouton de souris relâché
	virtual void sourisRelachee( EvenementSouris& evenementSouris);
	/// Comportement lorsque la souris est déplacée
	virtual void sourisDeplacee( EvenementSouris& evenementSouris);
	/// Comportement lorsqu'on bouge la roulette de la souris
	virtual void rouletteSouris(EvenementRouletteSouris& evenementRouletteSouris);
	/// Animations pour le mode jeu
	virtual void animer( const float& temps );

	/// Permet d'effectuer l'affichage specifique a l'etat
	virtual void afficher();
	virtual void miseAJourEvenementsRepetitifs( float deltaTemps );

private:

    GamesContainer games;
};

#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////