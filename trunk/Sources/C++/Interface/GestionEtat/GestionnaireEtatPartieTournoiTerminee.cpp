//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEtatPartieTournoiTerminee.cpp
/// @author Vincent Lemire, Michael Ferris
/// @date 2012-04-13
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
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
#include "AnimationCamera.h"
#include "ManagerAnimations.h"


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
GestionnaireEtatPartieTournoiTerminee::GestionnaireEtatPartieTournoiTerminee(): GestionnaireEtatAbstrait()
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
	if(evenementClavier.obtenirTouche() == VJAK_R ||  evenementClavier.obtenirTouche() == VJAK_CONTROL /*|| GestionnaireAnimations::obtenirInstance()->estJouerReplay()*/)
		return;
// 	GestionnaireAnimations::obtenirInstance()->terminerReplay();
// 	GestionnaireAnimations::obtenirInstance()->viderBufferReplay();

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
	EventManager::modifierEtat(ETAT_MODE_TOURNOI);
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
    auto game = FacadeModele::getInstance()->obtenirPartieCourante();
    if(game)
    {
        renderBase(game->getField(),[&]() -> void{game->afficher();});
    }
// 	if( GestionnaireAnimations::obtenirInstance()->estJouerReplay() )
// 		GestionnaireHUD::obtenirInstance()->dessinerHUDJeu();
// 	else
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
    // Ajout de l'animation de camera pour pointer sur le tableau du tournoi
    vue::Camera* cameraCourante = &FacadeModele::getInstance()->obtenirVue()->obtenirCamera();
    // On enleve toutes les animation de camera precedente
    cameraCourante->AnimationSubject::signalObservers();

    auto anim = ManagerAnimations::obtenirInstance()->CreateAnimation<AnimationCamera>();
    AnimationCamera* cameraAnim = anim.second;

    cameraAnim->AddFrame(0, cameraCourante->obtenirPosition(),cameraCourante->obtenirPointVise(), cameraCourante->obtenirDirectionHaut());
    cameraAnim->AddFrame(1, Vecteur3(-925, 0, 480),  Vecteur3(-1200, 0, 480), Vecteur3(0, 0, 1));
    cameraAnim->AddFrame(1.2f, Vecteur3(-925, 0, 480),  Vecteur3(-1200, 0, 480), Vecteur3(0, 0, 1));
    cameraAnim->SetCamera(cameraCourante);
    cameraAnim->SetAlgo(ANIMATION_LINEAR);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatPartieTournoiTerminee::updateObserver( const ReplaySubject* pSubject )
///
/// /*Description*/
///
/// @param[in] const ReplaySubject * pSubject
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatPartieTournoiTerminee::updateObserver( const ReplaySubject* pSubject )
{
    if(!pSubject->mReplaying)
    {
        pointerCameraSurScoreBoard();
    }
}
