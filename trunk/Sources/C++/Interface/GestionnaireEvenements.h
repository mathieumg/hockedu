//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEvenements.h
/// @author Vincent Lemire
/// @date 2012-01-20
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef GESTIONNAIREEVENEMENTS_H
#define GESTIONNAIREEVENEMENTS_H

#include "Singleton.h"
#include "EvenementClavier.h"
#include "EvenementRouletteSouris.h"
#include "EvenementSouris.h"
#include "MouseMoveObserver.h"

class GestionnaireEvenementsTest;
class GestionnaireEtatAbstrait;


enum EtatGestion{
	ETAT_MENU_PRINCIPAL,
	ETAT_MODE_EDITION,
	ETAT_MODE_JEU,
	ETAT_MODE_TOURNOI,
	ETAT_PARTIE_RAPIDE_TERMINEE,
	ETAT_PARTIE_TOURNOI_TERMINEE
};

////////////////////////////////////////////////////////////////////////////
/// @class GestionnaireEvenements
/// @brief Classe qui redirige les événements au gestionnaire appropié selon
///	l'état courant.
///
/// @author Vincent Lemire
/// @date 2012-01-20
////////////////////////////////////////////////////////////////////////////
class GestionnaireEvenements:public Singleton<GestionnaireEvenements>, public MouseMoveSubject
{
	
public:
	/// Utile pour les tests
	friend GestionnaireEvenementsTest;

	/// Modifie l'état
	void modifierEtat(const EtatGestion &nouvelEtat);

	/// Redirige l'événement d'une touche enfoncée
	void toucheEnfoncee(EvenementClavier& evenementClavier);
	/// Redirige l'événement d'une touche relachée
	void toucheRelachee(EvenementClavier& evenementClavier);
	/// Redirige l'événement d'un bouton de souris enfoncé
	void sourisEnfoncee(EvenementSouris& evenementSouris);
	/// Redirige l'événement d'un bouton de souris relâché
	void sourisRelachee(EvenementSouris& evenementSouris);
	/// Redirige l'événement de deplacement de la souris
	void sourisDeplacee(EvenementSouris& evenementSouris);
	/// Redirige l'événement de roulette de souris
	void rouletteSouris(EvenementRouletteSouris& evenementRouletteSouris);

	void miseAJourEvenementsRepetitifs(float detlaTemps);
	/// Renvoie l'état courant
	GestionnaireEtatAbstrait* obtenirEtat();
private:
	SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(GestionnaireEvenements);
	/// Contructeur
	GestionnaireEvenements();
	/// Destructeur
	~GestionnaireEvenements();
	/// Ceci est un pointeur vers le gestionnaire d'événement auquel renvoyer les différents événements, comme le clavier ou la souris, selon l'état courant
	GestionnaireEtatAbstrait* etatCourant_; 

};

#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


