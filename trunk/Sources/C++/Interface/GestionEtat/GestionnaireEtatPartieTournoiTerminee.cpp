//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEtatPartieTournoiTerminee.cpp
/// @author Vincent Lemire, Michael Ferris
/// @date 2012-04-13
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////
#include "GestionnaireEtatPartieTournoiTerminee.h"
#include "GestionnaireHUD.h"
#include "FacadeModele.h"
#include "Partie.h"
#include "SoundFMOD.h"
#include "Tournoi.h"
#include "Camera.h"
#include "Vue.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn  GestionnaireEtatPartieTournoiTerminee::GestionnaireEtatPartieTournoiTerminee( void )
///
/// Constructeur
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GestionnaireEtatPartieTournoiTerminee::GestionnaireEtatPartieTournoiTerminee(GestionnaireEvenements* contexte): GestionnaireEtatAbstrait(contexte)
{
	pointerCameraSurScoreBoard();
	Partie* partie = FacadeModele::getInstance()->obtenirPartieCourante();
	if(partie)
		partie->obtenirGameTime()->pause();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn  GestionnaireEtatPartieTournoiTerminee::~GestionnaireEtatPartieTournoiTerminee( void )
///
/// Destructeur
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GestionnaireEtatPartieTournoiTerminee::~GestionnaireEtatPartieTournoiTerminee(void)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatPartieTournoiTerminee::toucheEnfoncee( EvenementClavier& evenementClavier )
///
/// Comportement pour une touche enfoncée
///
/// @param[in] EvenementClavier & evenementClavier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatPartieTournoiTerminee::toucheEnfoncee( EvenementClavier& evenementClavier )
{
	if(evenementClavier.obtenirTouche() == VJAK_R || GestionnaireAnimations::obtenirInstance()->estJouerReplay())
		return;
	GestionnaireAnimations::obtenirInstance()->terminerReplay();
	GestionnaireAnimations::obtenirInstance()->viderBufferReplay();

	Tournoi* tournoi = FacadeModele::getInstance()->obtenirTournoi();
	if(tournoi->estTermine())
	{
		// Demander pour Rejouer le tournoi
		tournoi->reinitialiserTournoi();
		FacadeModele::getInstance()->enregistrerTournoi(tournoi);
		FacadeModele::getInstance()->passageModeTournoi();
	}
	else
	{
		FacadeModele::getInstance()->passageModeTournoi();
	}
	tournoi->obtenirPartieCourante()->modifierEnPause(false);
	contexte_->modifierEtat(ETAT_MODE_TOURNOI);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatPartieTournoiTerminee::toucheRelachee( EvenementClavier& evenementClavier )
///
/// Comportement pour une touche relachée
///
/// @param[in] EvenementClavier & evenementClavier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatPartieTournoiTerminee::toucheRelachee( EvenementClavier& evenementClavier )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatPartieTournoiTerminee::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Comportement pour un bouton de souris enfoncé
///
/// @param[in] EvenementSouris & evenementSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatPartieTournoiTerminee::sourisEnfoncee( EvenementSouris& evenementSouris )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatPartieTournoiTerminee::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Comportement pour un bouton de souris relâché
///
/// @param[in] EvenementSouris & evenementSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatPartieTournoiTerminee::sourisRelachee( EvenementSouris& evenementSouris )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatPartieTournoiTerminee::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Comportement lorsque la souris est déplacée
///
/// @param[in] EvenementSouris & evenementSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatPartieTournoiTerminee::sourisDeplacee( EvenementSouris& evenementSouris )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatPartieTournoiTerminee::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
///
/// Comportement lorsqu'on bouge la roulette de la souris
///
/// @param[in] EvenementRouletteSouris & evenementRouletteSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatPartieTournoiTerminee::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatPartieTournoiTerminee::animer( const float& temps )
///
/// Comportement de la methode animer 
///
/// @param[in] const float & temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatPartieTournoiTerminee::animer( const float& temps )
{
	SoundFMOD::obtenirInstance()->repeatAppSong();
	if(GestionnaireAnimations::obtenirInstance()->replayEstTermine())
	{
		pointerCameraSurScoreBoard();
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatPartieTournoiTerminee::afficher()
///
/// Permet d'effectuer l'affichage specifique a l'etat
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatPartieTournoiTerminee::afficher()
{
	if( GestionnaireAnimations::obtenirInstance()->estJouerReplay() )
		GestionnaireHUD::obtenirInstance()->dessinerHUDJeu();
	else
		GestionnaireHUD::obtenirInstance()->dessinerHUDTournoi();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatPartieTournoiTerminee::pointerCameraSurScoreBoard()
///
/// On vise la camera sur le tableau
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatPartieTournoiTerminee::pointerCameraSurScoreBoard()
{
	// On enleve toutes les animation de camera precedente
	GestionnaireAnimations::obtenirInstance()->viderAnimationCamera();

	// Ajout de l'animation de camera pour pointer sur le tableau du tournoi
	vue::Camera* cameraCourante = &FacadeModele::getInstance()->obtenirVue()->obtenirCamera();

	AnimationFrame* frame[3];
	frame[0] = new AnimationFrame(0, cameraCourante->obtenirPosition(), cameraCourante->obtenirPointVise(), cameraCourante->obtenirDirectionHaut());
	frame[1] = new AnimationFrame(1000, Vecteur3(-925, 0, 480),  Vecteur3(-1200, 0, 480), Vecteur3(0, 0, 1));
	frame[2] = new AnimationFrame(1200, Vecteur3(-925, 0, 480),  Vecteur3(-1200, 0, 480), Vecteur3(0, 0, 1));

	Animation* animation = new Animation(LINEAIRE, true, true, true);
	for(int i=0; i<3; i++)
		animation->ajouterFrame(frame[i]);

	animation->ajouterObjet(cameraCourante);
	GestionnaireAnimations::obtenirInstance()->ajouterAnimation(animation);
}
