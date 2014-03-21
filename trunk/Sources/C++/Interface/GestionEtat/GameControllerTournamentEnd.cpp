//////////////////////////////////////////////////////////////////////////////
/// @file GameControllerTournamentEnd.cpp
/// @author Vincent Lemire, Michael Ferris
/// @date 2012-04-13
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#include "GameControllerTournamentEnd.h"
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
/// @fn  GameControllerTournamentEnd::GameControllerTournamentEnd( void )
///
/// Constructeur
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GameControllerTournamentEnd::GameControllerTournamentEnd(): GameControllerAbstract()
{
	pointerCameraSurScoreBoard();
	Partie* partie = FacadeModele::getInstance()->obtenirPartieCourante();
	if(partie)
		partie->obtenirGameTime()->pause();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn  GameControllerTournamentEnd::~GameControllerTournamentEnd( void )
///
/// Destructeur
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GameControllerTournamentEnd::~GameControllerTournamentEnd(void)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerTournamentEnd::toucheEnfoncee( EvenementClavier& evenementClavier )
///
/// Comportement pour une touche enfoncée
///
/// @param[in] EvenementClavier & evenementClavier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerTournamentEnd::toucheEnfoncee( EvenementClavier& evenementClavier )
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
		FacadeModele::getInstance()->changeControler(GAME_CONTROLLER_TOURNAMENT);
	}
	else
	{
        FacadeModele::getInstance()->changeControler( GAME_CONTROLLER_TOURNAMENT );
	}
	tournoi->obtenirPartieCourante()->modifierEnPause(false);
	EventManager::modifierEtat(ETAT_MODE_TOURNOI);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerTournamentEnd::toucheRelachee( EvenementClavier& evenementClavier )
///
/// Comportement pour une touche relachée
///
/// @param[in] EvenementClavier & evenementClavier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerTournamentEnd::toucheRelachee( EvenementClavier& evenementClavier )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerTournamentEnd::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Comportement pour un bouton de souris enfoncé
///
/// @param[in] EvenementSouris & evenementSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerTournamentEnd::sourisEnfoncee( EvenementSouris& evenementSouris )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerTournamentEnd::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Comportement pour un bouton de souris relâché
///
/// @param[in] EvenementSouris & evenementSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerTournamentEnd::sourisRelachee( EvenementSouris& evenementSouris )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerTournamentEnd::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Comportement lorsque la souris est déplacée
///
/// @param[in] EvenementSouris & evenementSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerTournamentEnd::sourisDeplacee( EvenementSouris& evenementSouris )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerTournamentEnd::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
///
/// Comportement lorsqu'on bouge la roulette de la souris
///
/// @param[in] EvenementRouletteSouris & evenementRouletteSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerTournamentEnd::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerTournamentEnd::animer( const float& temps )
///
/// Comportement de la methode animer 
///
/// @param[in] const float & temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerTournamentEnd::animer( const float& temps )
{
	SoundFMOD::obtenirInstance()->repeatAppSong();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerTournamentEnd::afficher()
///
/// Permet d'effectuer l'affichage specifique a l'etat
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerTournamentEnd::afficher()
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
/// @fn void GameControllerTournamentEnd::pointerCameraSurScoreBoard()
///
/// On vise la camera sur le tableau
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerTournamentEnd::pointerCameraSurScoreBoard()
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
/// @fn void GameControllerTournamentEnd::updateObserver( const ReplaySubject* pSubject )
///
/// /*Description*/
///
/// @param[in] const ReplaySubject * pSubject
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerTournamentEnd::updateObserver( const ReplaySubject* pSubject )
{
    if(!pSubject->mReplaying)
    {
        pointerCameraSurScoreBoard();
    }
}
