//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEtatModeTournoi.cpp
/// @author Vincent Lemire
/// @date 2012-03-02
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "GestionnaireEtatModeTournoi.h"
#include "FacadeModele.h"
#include "ConfigScene.h"
#include "Partie.h"
#include "Terrain.h"

#include "GestionnaireHUD.h"
#include "Tournoi.h"
#include "SoundFMOD.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn GestionnaireEtatModeTournoi::GestionnaireEtatModeTournoi(GestionnaireEvenements* contexte) : GestionnaireEtatAbstrait(contexte)
///
/// Constructeur qui initialise le contexte et mets un événement de souris par défaut.
///
/// @param[in] touche : Le nombre de coches parcouru sur la roulette.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
GestionnaireEtatModeTournoi::GestionnaireEtatModeTournoi(Tournoi* tournoi) : GestionnaireEtatAbstrait(), mTournoi(tournoi)
{
	modifierEtatSouris(ETAT_SOURIS_DEPLACER_FENETRE);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeTournoi::toucheEnfoncee( EvenementClavier& evenementClavier )
///
/// Fonction qui gère les événements liés à l'enfoncement d'une touche.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeTournoi::toucheEnfoncee( EvenementClavier& evenementClavier )
{
    ToucheClavier touche = evenementClavier.obtenirTouche();
	
    if(touche == VJAK_SPACE)
    {
        FacadeModele::getInstance()->togglePause();
    }
	else
    {
		toucheSauvegardee_ = touche;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeTournoi::toucheRelachee( EvenementClavier& evenementClavier )
///
/// Fonction qui gère les événements liés au relâchement d'une touche.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeTournoi::toucheRelachee( EvenementClavier& evenementClavier )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeTournoi::sourisEnfoncee(EvenementSouris& evenementSouris)
///
/// Fonction qui gère les événements liés à l'enfoncement d'un bouton de la souris.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeTournoi::sourisEnfoncee( EvenementSouris& evenementSouris )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeTournoi::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Fonction qui gère les événements liés au relâchement d'un bouton de la souris.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeTournoi::sourisRelachee( EvenementSouris& evenementSouris )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeTournoi::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Fonction qui gère les événements liés au déplacement d'un bouton de la souris.
///
/// @param[in] evenementSouris : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeTournoi::sourisDeplacee( EvenementSouris& evenementSouris )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeTournoi::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
///
/// Fonction qui gère les événements liés au déplacement de la roulette de la souris
///
/// @param[in] evenementRouletteSouris : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeTournoi::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
{
	GestionnaireEtatAbstrait::rouletteSouris(evenementRouletteSouris);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeTournoi::animer( const float& temps )
///
/// /*Description*/
///
/// @param[in] const float & temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeTournoi::animer( const float& temps )
{
	SoundFMOD::obtenirInstance()->change_song_if_end();

	Partie* partieCourante = FacadeModele::getInstance()->obtenirPartieCourante();
	
	if(partieCourante)
	{
		partieCourante->animer(temps);
		partieCourante->updateMinuterie((int)(temps*1000));
		if(!FacadeModele::getInstance()->estEnPause() /*&& !GestionnaireAnimations::obtenirInstance()->estJouerReplay()*/)
		{
			Tournoi* tournoi = FacadeModele::getInstance()->obtenirTournoi();
			if(tournoi->miseAJour())
			{
				EventManager::modifierEtat(ETAT_PARTIE_TOURNOI_TERMINEE);
				return;
			}
		}
        gestionAnimationEnJeu(partieCourante, temps);
	}

	

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeTournoi::afficher()
///
/// Permet d'effectuer l'affichage specifique a l'etat
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeTournoi::afficher()
{
	Tournoi* tournoi = FacadeModele::getInstance()->obtenirTournoi();
    if(tournoi)
    {
        Partie* game = tournoi->obtenirPartieCourante();
        if(game)
        {
            renderBase(game->getField(),[&]()->void{game->afficher();});
            if(game->partieTerminee())
            {
                game->setGameStatus(GAME_ENDED);
                GestionnaireHUD::obtenirInstance()->dessinerHUDTournoi();
            }
            else
            {
                GestionnaireHUD::obtenirInstance()->dessinerHUDJeu();
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeTournoi::updateObserver( const ReplaySubject* pSubject )
///
/// /*Description*/
///
/// @param[in] const ReplaySubject * pSubject
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeTournoi::updateObserver( const ReplaySubject* pSubject )
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

        Partie* partieCourante = FacadeModele::getInstance()->obtenirPartieCourante();
        if(partieCourante)
            partieCourante->delais(partieCourante->getMiseAuJeuDelai());
    }
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
