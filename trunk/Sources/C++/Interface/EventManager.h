//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEvenements.h
/// @author Vincent Lemire
/// @date 2012-01-20
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef GESTIONNAIREEVENEMENTS_H
#define GESTIONNAIREEVENEMENTS_H

#include "EvenementClavier.h"
#include "EvenementRouletteSouris.h"
#include "EvenementSouris.h"
#include "Flags.h"

class GestionnaireEvenementsTest;
class GestionnaireEtatAbstrait;


enum EtatGestion{
	ETAT_MENU_PRINCIPAL,
	ETAT_MODE_EDITION,
	ETAT_MODE_JEU,
	ETAT_MODE_TOURNOI,
	ETAT_PARTIE_RAPIDE_TERMINEE,
    ETAT_PARTIE_TOURNOI_TERMINEE,
    ETAT_MODE_SIMULATION,
};

enum CameraPressedDirections
{
	CAMERA_UP,
	CAMERA_DOWN,
	CAMERA_LEFT,
	CAMERA_RIGHT,

	NB_CAMERA_PRESSED_DIRECTIONS
};

enum MalletPressedDirections
{
	MALLET_UP,
	MALLET_DOWN,
	MALLET_LEFT,
	MALLET_RIGHT,

	NB_MALLET_PRESSED_DIRECTIONS
};

////////////////////////////////////////////////////////////////////////////
/// @class EventManager
/// @brief Classe qui redirige les événements au gestionnaire appropié selon
///         l'état courant du client.
///
/// @author Vincent Lemire
/// @date 2012-01-20
////////////////////////////////////////////////////////////////////////////
class EventManager
{
	
public:
	/// Utile pour les tests
	friend GestionnaireEvenementsTest;

	/// Modifie l'état
	static void modifierEtat(const EtatGestion &nouvelEtat);

	/// Redirige l'événement d'une touche enfoncée
	static void toucheEnfoncee(EvenementClavier& evenementClavier);
	/// Redirige l'événement d'une touche relachée
	static void toucheRelachee(EvenementClavier& evenementClavier);
	/// Redirige l'événement d'un bouton de souris enfoncé
	static void sourisEnfoncee(EvenementSouris& evenementSouris);
	/// Redirige l'événement d'un bouton de souris relâché
	static void sourisRelachee(EvenementSouris& evenementSouris);
	/// Redirige l'événement de deplacement de la souris
	static void sourisDeplacee(EvenementSouris& evenementSouris);
	/// Redirige l'événement de roulette de souris
	static void rouletteSouris(EvenementRouletteSouris& evenementRouletteSouris);

	static void miseAJourEvenementsRepetitifs(float detlaTemps);
	static bool IsKeyPressed(ToucheClavier key);
    /// Modifier l'état de la souris
    static void modifierEtatSouris(enum NomEtatSouris etatSouris);
    static void afficher();
    static void animer(float temps);

	/// Ceci est un pointeur vers le gestionnaire d'événement auquel renvoyer les différents événements, comme le clavier ou la souris, selon l'état courant
	static GestionnaireEtatAbstrait* etatCourant_; 

	/// flags used to cache key pressed for camera movement and mallet movement
	static Flags<char,NB_CAMERA_PRESSED_DIRECTIONS> mCameraDirection;
	static Flags<char,NB_MALLET_PRESSED_DIRECTIONS> mMalletDirection;
	static Vecteur2i mMouseScreenPos;
	static Vecteur3 mMouseGamePos;
private:
    EventManager(){}
    static class GameTime* mDoubleClickTimer;
};

#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


