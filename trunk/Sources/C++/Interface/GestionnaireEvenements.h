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
/// @brief Classe qui redirige les �v�nements au gestionnaire appropi� selon
///         l'�tat courant du client.
///
/// @author Vincent Lemire
/// @date 2012-01-20
////////////////////////////////////////////////////////////////////////////
class GestionnaireEvenements
{
	
public:
	/// Utile pour les tests
	friend GestionnaireEvenementsTest;

	/// Modifie l'�tat
	static void modifierEtat(const EtatGestion &nouvelEtat);

	/// Redirige l'�v�nement d'une touche enfonc�e
	static void toucheEnfoncee(EvenementClavier& evenementClavier);
	/// Redirige l'�v�nement d'une touche relach�e
	static void toucheRelachee(EvenementClavier& evenementClavier);
	/// Redirige l'�v�nement d'un bouton de souris enfonc�
	static void sourisEnfoncee(EvenementSouris& evenementSouris);
	/// Redirige l'�v�nement d'un bouton de souris rel�ch�
	static void sourisRelachee(EvenementSouris& evenementSouris);
	/// Redirige l'�v�nement de deplacement de la souris
	static void sourisDeplacee(EvenementSouris& evenementSouris);
	/// Redirige l'�v�nement de roulette de souris
	static void rouletteSouris(EvenementRouletteSouris& evenementRouletteSouris);

	static void miseAJourEvenementsRepetitifs(float detlaTemps);
	/// Renvoie l'�tat courant
	GestionnaireEtatAbstrait* obtenirEtat();

    /// Modifier l'�tat de la souris
    static void modifierEtatSouris(enum NomEtatSouris etatSouris);
    static void afficher();
    static void animer(float temps);

	/// Ceci est un pointeur vers le gestionnaire d'�v�nement auquel renvoyer les diff�rents �v�nements, comme le clavier ou la souris, selon l'�tat courant
	static GestionnaireEtatAbstrait* etatCourant_; 

private:
    GestionnaireEvenements(){}
};

#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


