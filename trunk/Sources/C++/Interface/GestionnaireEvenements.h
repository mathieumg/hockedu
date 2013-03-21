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



////////////////////////////////////////////////////////////////////////////
/// @class GestionnaireEvenements
/// @brief Classe qui redirige les événements au gestionnaire appropié selon
///         l'état courant du client.
///
/// @author Vincent Lemire
/// @date 2012-01-20
////////////////////////////////////////////////////////////////////////////
class GestionnaireEvenements
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
	/// Renvoie l'état courant
	GestionnaireEtatAbstrait* obtenirEtat();

    /// Modifier l'état de la souris
    static void modifierEtatSouris(enum NomEtatSouris etatSouris);
    static void afficher();
    static void animer(float temps);

	/// Ceci est un pointeur vers le gestionnaire d'événement auquel renvoyer les différents événements, comme le clavier ou la souris, selon l'état courant
	static GestionnaireEtatAbstrait* etatCourant_; 

private:
    GestionnaireEvenements(){}
};

#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


