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
/// @brief Classe qui redirige les �v�nements au gestionnaire appropi� selon
///	l'�tat courant.
///
/// @author Vincent Lemire
/// @date 2012-01-20
////////////////////////////////////////////////////////////////////////////
class GestionnaireEvenements:public Singleton<GestionnaireEvenements>, public MouseMoveSubject
{
	
public:
	/// Utile pour les tests
	friend GestionnaireEvenementsTest;

	/// Modifie l'�tat
	void modifierEtat(const EtatGestion &nouvelEtat);

	/// Redirige l'�v�nement d'une touche enfonc�e
	void toucheEnfoncee(EvenementClavier& evenementClavier);
	/// Redirige l'�v�nement d'une touche relach�e
	void toucheRelachee(EvenementClavier& evenementClavier);
	/// Redirige l'�v�nement d'un bouton de souris enfonc�
	void sourisEnfoncee(EvenementSouris& evenementSouris);
	/// Redirige l'�v�nement d'un bouton de souris rel�ch�
	void sourisRelachee(EvenementSouris& evenementSouris);
	/// Redirige l'�v�nement de deplacement de la souris
	void sourisDeplacee(EvenementSouris& evenementSouris);
	/// Redirige l'�v�nement de roulette de souris
	void rouletteSouris(EvenementRouletteSouris& evenementRouletteSouris);

	void miseAJourEvenementsRepetitifs(float detlaTemps);
	/// Renvoie l'�tat courant
	GestionnaireEtatAbstrait* obtenirEtat();
private:
	SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(GestionnaireEvenements);
	/// Contructeur
	GestionnaireEvenements();
	/// Destructeur
	~GestionnaireEvenements();
	/// Ceci est un pointeur vers le gestionnaire d'�v�nement auquel renvoyer les diff�rents �v�nements, comme le clavier ou la souris, selon l'�tat courant
	GestionnaireEtatAbstrait* etatCourant_; 

};

#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


