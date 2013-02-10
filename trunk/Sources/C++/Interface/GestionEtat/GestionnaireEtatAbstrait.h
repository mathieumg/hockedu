//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEtatAbstrait.h
/// @author Vincent Lemire
/// @date 2012-01-20
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GestionnaireEvenements.h"
#include "CodesEvenements.h"

class GestionnaireEvenementsTest;
class GestionnaireEvenements;
class SourisEtatAbstrait;
class Partie;

enum NomEtatSouris{
	ETAT_SOURIS_DEPLACER_FENETRE			
	,ETAT_SOURIS_ZOOM_ELASTIQUE				
	,ETAT_SOURIS_ZOOM_PROPORTIONNEL			
	,ETAT_SOURIS_TRANSFORMATION_ROTATION	
	,ETAT_SOURIS_TRANSFORMATION_ECHELLE		
	,ETAT_SOURIS_TRANSFORMATION_DEPLACEMENT	
	,ETAT_SOURIS_SELECTION					
	,ETAT_SOURIS_AJOUTER_PORTAIL			
	,ETAT_SOURIS_AJOUTER_MURET				
	,ETAT_SOURIS_AJOUTER_MAILLET			
	,ETAT_SOURIS_AJOUTER_RONDELLE			
	,ETAT_SOURIS_AJOUTER_ACCELERATEUR
	,ETAT_SOURIS_ORBIT
	,NB_ETATS_SOURIS
	,ETAT_SOURIS_INCONNU
};
////////////////////////////////////////////////////////////////////////////
/// @class GestionnaireEtatAbstrait
/// @brief Classe abstraite dont les h�riti�res gereront les �tats selon le
/// patron state.
///
/// @author Vincent Lemire, Michael Ferris
/// @date 2012-01-20
////////////////////////////////////////////////////////////////////////////
class GestionnaireEtatAbstrait
{
public:
	/// Requis pour l'accomplissement de tests.
	friend GestionnaireEvenementsTest;

	/// Constructeur par param�tre.
	GestionnaireEtatAbstrait(GestionnaireEvenements* contexte);

	/// Destructeur
	virtual ~GestionnaireEtatAbstrait();

	/// Comportement pour une touche enfonc�e
	virtual void toucheEnfoncee(EvenementClavier& evenementClavier) = 0;
	/// Comportement pour une touche relach�e
	virtual void toucheRelachee(EvenementClavier& evenementClavier) = 0;
	/// Comportement pour un bouton de souris enfonc�
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris) = 0;
	/// Comportement pour un bouton de souris rel�ch�
	virtual void sourisRelachee( EvenementSouris& evenementSouris) = 0;
	/// Comportement lorsque la souris est d�plac�e
	virtual void sourisDeplacee( EvenementSouris& evenementSouris) = 0;
	/// Comportement lorsqu'on bouge la roulette de la souris
	virtual void rouletteSouris(EvenementRouletteSouris& evenementRouletteSouris) = 0;
	/// Comportement de la methode animer 
	virtual void animer( const float& temps ) = 0;
	/// Permet d'effectuer l'affichage specifique a l'etat
	virtual void afficher(){}


	/// Modifier l'�tat de la souris
	void modifierEtatSouris(NomEtatSouris etatSouris);
	/// Met a jour les evenements clavier pour la repetition
	virtual void miseAJourEvenementsRepetitifs(float deltaTemps);

	/// Pour les tests
	GestionnaireEvenements* obtenirContexte() const { return contexte_; }

	/// Gere les animations communes pour les modes tounoir et partie rapide
	void gestionAnimationEnJeu( Partie* partieCourante, const float& temps );

	SourisEtatAbstrait* obtenirEtatSouris(){return etatSouris_;}

    // retourne vrai si on arrive a retrouver un nom valid
    bool GetMouseStateName(NomEtatSouris& outName);
    
protected:
	/// �tat de la souris
	SourisEtatAbstrait* etatSouris_;

	ToucheClavier toucheSauvegardee_;
	/// R�f�rence sur le gestionnaire d'�v�nements qui contient l'�tat du programme 
	GestionnaireEvenements* contexte_;
	
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


