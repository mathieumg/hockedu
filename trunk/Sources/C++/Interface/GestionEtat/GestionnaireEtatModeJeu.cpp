//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEtatModeJeu.cpp
/// @author Vincent Lemire
/// @date 2012-01-20
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "GestionnaireEtatModeJeu.h"
#include "FacadeModele.h"
#include "ConfigScene.h"

#include "Partie.h"
#include "Terrain.h"
#include "GestionnaireHUD.h"
#include "GestionnaireEtatAbstrait.h"
#include "SourisEtatAbstrait.h"
#include "SoundFMOD.h"
#include "FacadeCSharp.h"




////////////////////////////////////////////////////////////////////////
///
/// @fn  GestionnaireEtatModeJeu::GestionnaireEtatModeJeu( GestionnaireEvenements* contexte )
///
/// Constructeur qui initialise les valeurs requises pour faire fonctionner le gestionnaire. 
/// Place l'état par défaut à Sélection.
///
/// @param[in] GestionnaireEvenements * contexte
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GestionnaireEtatModeJeu::GestionnaireEtatModeJeu( int pGameId ):
GestionnaireEtatAbstrait(),mGameId(pGameId)
{
	modifierEtatSouris(ETAT_SOURIS_DEPLACER_FENETRE);
	shiftEnfonce_ = false;
	enfonce_ = false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  GestionnaireEtatModeJeu::~GestionnaireEtatModeJeu()
///
/// Destructeur virtuel
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
GestionnaireEtatModeJeu::~GestionnaireEtatModeJeu()
{

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeJeu::toucheEnfoncee(EvenementClavier& evenementClavier)
///
/// Fonction qui gère les événements liés à l'enfoncement d'une touche.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeJeu::toucheEnfoncee(EvenementClavier& evenementClavier)
{
	ToucheClavier touche = evenementClavier.obtenirTouche();

	if(touche == VJAK_F1)
    {
        TestTrajectoryPredictionDLL();
    }

	// VERIF ETAT PAUSE
	if(touche==VJAK_SHIFT)
    {
		shiftEnfonce_ = true;
    }
    else if(touche == VJAK_SPACE)
    {
        FacadeModele::getInstance()->togglePause();
        /*if(wGame)
        {
            wGame->modifierEnPause(!wGame->estEnPause());
        }*/
    }
	else
    {
		toucheSauvegardee_ = touche;
    }
    
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeJeu::toucheRelachee(EvenementClavier& evenementClavier)
///
/// Fonction qui gère les événements liés au relâchement d'une touche.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeJeu::toucheRelachee( EvenementClavier& evenementClavier )
{
	ToucheClavier touche = evenementClavier.obtenirTouche();

	// VERIF ETAT PAUSE

	if (etatSouris_)
	{
		etatSouris_ ->toucheRelachee(evenementClavier);
	}
	if(touche==VJAK_SHIFT)
		shiftEnfonce_ = false;
	if(toucheSauvegardee_==touche)
		toucheSauvegardee_ = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeJeu::sourisEnfoncee(EvenementSouris& evenementSouris)
///
/// Fonction qui gère les événements liés à l'enfoncement d'un bouton de la souris.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeJeu::sourisEnfoncee( EvenementSouris& evenementSouris )
{
	// VERIF ETAT PAUSE
	if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_MILIEU)
		enfonce_ = true;
	positionSouris_ = evenementSouris.obtenirPosition();
	boutonEnfonce_ = evenementSouris.obtenirBouton();
    if (etatSouris_)
    {
        etatSouris_->sourisEnfoncee(evenementSouris);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeJeu::sourisRelachee(EvenementSouris& evenementSouris)
///
/// Fonction qui gère les événements liés au relâchement d'un bouton de la souris.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeJeu::sourisRelachee( EvenementSouris& evenementSouris )
{
	// VERIF ETAT PAUSE
	if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_MILIEU)
		enfonce_ = false;
    if (etatSouris_)
    {
        etatSouris_->sourisRelachee(evenementSouris);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeJeu::sourisDeplacee(EvenementSouris& evenementSouris)
///
/// Fonction qui gère les événements liés au déplacement d'un bouton de la souris.
///
/// @param[in] evenementSouris : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeJeu::sourisDeplacee( EvenementSouris& evenementSouris )
{
	Vecteur3 coordonneesSouris, anciennePos;
	FacadeModele::getInstance()->convertirClotureAVirtuelle(evenementSouris.obtenirPosition()[VX], evenementSouris.obtenirPosition()[VY], coordonneesSouris);
	
	// VERIF ETAT PAUSE
	if(enfonce_ && boutonEnfonce_==BOUTON_SOURIS_MILIEU)
	{
		Vecteur2i delta = evenementSouris.obtenirPosition()-positionSouris_;
		if(shiftEnfonce_)
		{
			FacadeModele::getInstance()->deplacerSouris(delta);

		}
		else
		{
			FacadeModele::getInstance()->orbit(delta);
		}
		positionSouris_ = evenementSouris.obtenirPosition();
	}
	
    if (etatSouris_)
    {
        etatSouris_->sourisDeplacee(evenementSouris);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeJeu::rouletteSouris(EvenementRouletteSouris& evenementRouletteSouris)
///
/// Fonction qui gère les événements liés au déplacement de la roulette de la souris
///
/// @param[in] evenementRouletteSouris : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeJeu::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
{
	GestionnaireEtatAbstrait::rouletteSouris(evenementRouletteSouris);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeJeu::animer( const float& temps )
///
/// Animation du mode jeu.
///
/// @param[in] const float & temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeJeu::animer( const float& temps )
{
	SoundFMOD::obtenirInstance()->change_song_if_end();
	Partie* wGame = GameManager::obtenirInstance()->getGame(mGameId);

	if(wGame)
	{
		wGame->animer(temps);
		
        if(wGame->partieTerminee())
        {
            wGame->setGameStatus(GAME_ENDED);
            EventManager::modifierEtat(ETAT_PARTIE_RAPIDE_TERMINEE);
            return;
        }
        gestionAnimationEnJeu(wGame, temps);
	}
	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeJeu::afficher()
///
/// Permet d'effectuer l'affichage specifique a l'etat
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeJeu::afficher()
{
    Partie* wGame = GameManager::obtenirInstance()->getGame(mGameId);
    if(wGame)
    {
        renderBase(wGame->getField(),[&]() -> void{wGame->afficher();});
    }
    GestionnaireHUD::obtenirInstance()->dessinerHUDJeu();
}

void GestionnaireEtatModeJeu::miseAJourEvenementsRepetitifs( float deltaTemps )
{
	GestionnaireEtatAbstrait::CameraMovementFromKeyPressed(deltaTemps);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeJeu::updateObserver( const ReplaySubject* pSubject )
///
/// /*Description*/
///
/// @param[in] const ReplaySubject * pSubject
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeJeu::updateObserver( const ReplaySubject* pSubject )
{
    if(!pSubject->mReplaying)
    {
            // TODO:: refactor Animation
//         vue::Camera ancienneCamera = GestionnaireAnimations::obtenirInstance()->obtenirAncienneCamera();
//         vue::Camera* camera = &FacadeModele::getInstance()->obtenirVue()->obtenirCamera();
// 
//         AnimationFrame* frame[2];
//         frame[0] = new AnimationFrame(0, camera->obtenirPosition(), camera->obtenirPointVise(), camera->obtenirDirectionHaut());
//         frame[1] = new AnimationFrame(500, ancienneCamera.obtenirPosition(), ancienneCamera.obtenirPointVise(), ancienneCamera.obtenirDirectionHaut());
// 
//         Animation* animation = new Animation(BEZIER, true, true, true);
//         for(int i=0; i<2; i++)
//             animation->ajouterFrame(frame[i]);
//         animation->ajouterObjet(camera);
//         GestionnaireAnimations::obtenirInstance()->ajouterAnimation(animation);
        Partie* wGame = GameManager::obtenirInstance()->getGame(mGameId);
        if(wGame)
            wGame->delais(wGame->getMiseAuJeuDelai());
    }
}










///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




