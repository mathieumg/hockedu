//////////////////////////////////////////////////////////////////////////////
/// @file GameControllerAbstract.h
/// @author Vincent Lemire
/// @date 2012-01-20
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "EventManager.h"
#include "CodesEvenements.h"
#include <functional>

class GestionnaireEvenementsTest;
class EventManager;
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
    ,ETAT_SOURIS_AJOUTER_BONUS
    ,ETAT_SOURIS_ORBIT
    ,ETAT_SOURIS_PIE_MODE
    ,ETAT_SOURIS_AUCUN
	,NB_ETATS_SOURIS
	,ETAT_SOURIS_INCONNU
};

//typedef void(*RenderSpecific)();
typedef std::function<void()> RenderSpecific;
////////////////////////////////////////////////////////////////////////////
/// @class GameControllerAbstract
/// @brief Classe abstraite dont les h�riti�res gereront les �tats selon le
/// patron state.
///
/// @author Vincent Lemire, Michael Ferris
/// @date 2012-01-20
////////////////////////////////////////////////////////////////////////////
class GameControllerAbstract
{
public:
	/// Requis pour l'accomplissement de tests.
	friend GestionnaireEvenementsTest;

	/// Constructeur par param�tre.
	GameControllerAbstract();

	/// Destructeur
	virtual ~GameControllerAbstract();

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
    static void renderBase(class Terrain* pField, RenderSpecific = nullptr);

	/// Modifier l'�tat de la souris
	virtual void modifierEtatSouris(NomEtatSouris etatSouris);
    /// clear mouse state memory if the newState is different from current
    void clearMouseState(NomEtatSouris newState);
	/// Met a jour les evenements clavier pour la repetition
	virtual void miseAJourEvenementsRepetitifs(float deltaTemps);

	/// Gere les animations communes pour les modes tournoi et partie rapide
	void gestionAnimationEnJeu( Partie* partieCourante, const float& temps );

	SourisEtatAbstrait* obtenirEtatSouris(){return etatSouris_;}

    // retourne vrai si on arrive a retrouver un nom valid
    bool GetMouseStateName(NomEtatSouris& outName);
	static void CameraMovementFromKeyPressed( float deltaTemps );
    void doubleClickEvent( EvenementSouris& evenementSouris );
protected:
	/// �tat de la souris
	SourisEtatAbstrait* etatSouris_;

	ToucheClavier toucheSauvegardee_;
	
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


