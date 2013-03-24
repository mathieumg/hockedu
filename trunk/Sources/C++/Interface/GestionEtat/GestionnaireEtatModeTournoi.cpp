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
#include "NoeudMaillet.h"
#include "FacadeModele.h"
#include "ConfigScene.h"
#include "Partie.h"
#include "Terrain.h"
#include "GestionnaireAnimations.h"
#include "GestionnaireHUD.h"
#include "Tournoi.h"
#include "SoundFMOD.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn GestionnaireEtatModeTournoi::GestionnaireEtatModeTournoi(GestionnaireEvenements* contexte) : GestionnaireEtatAbstrait(contexte)
///
/// Constructeur qui initialise le contexte et mets un �v�nement de souris par d�faut.
///
/// @param[in] touche : Le nombre de coches parcouru sur la roulette.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
GestionnaireEtatModeTournoi::GestionnaireEtatModeTournoi() : GestionnaireEtatAbstrait()
{
	modifierEtatSouris(ETAT_SOURIS_DEPLACER_FENETRE);
    GestionnaireAnimations::obtenirInstance()->attach(this);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeTournoi::toucheEnfoncee( EvenementClavier& evenementClavier )
///
/// Fonction qui g�re les �v�nements li�s � l'enfoncement d'une touche.
///
/// @param[in] evenementClavier : L'objet correspondant � l'�v�nement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeTournoi::toucheEnfoncee( EvenementClavier& evenementClavier )
{
	ToucheClavier touche = evenementClavier.obtenirTouche();
	NoeudMaillet* maillet = FacadeModele::getInstance()->obtenirMailletJoueurDroit();

	// Les 4 cas suivants d�placent le maillet du joueur 2
	if(touche == ConfigScene::obtenirInstance()->obtenirToucheHaut())
		maillet->modifierDirection(true,DIR_HAUT);

	if(touche == ConfigScene::obtenirInstance()->obtenirToucheGauche())
		maillet->modifierDirection(true,DIR_GAUCHE);

	if(touche == ConfigScene::obtenirInstance()->obtenirToucheDroite())
		maillet->modifierDirection(true,DIR_DROITE);

	if(touche == ConfigScene::obtenirInstance()->obtenirToucheBas())
		maillet->modifierDirection(true,DIR_BAS);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeTournoi::toucheRelachee( EvenementClavier& evenementClavier )
///
/// Fonction qui g�re les �v�nements li�s au rel�chement d'une touche.
///
/// @param[in] evenementClavier : L'objet correspondant � l'�v�nement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeTournoi::toucheRelachee( EvenementClavier& evenementClavier )
{
	ToucheClavier touche = evenementClavier.obtenirTouche();
	NoeudMaillet* maillet = FacadeModele::getInstance()->obtenirMailletJoueurDroit();

	// Les 4 cas suivants d�placent le maillet du joueur 2
	if(touche == ConfigScene::obtenirInstance()->obtenirToucheHaut())
		maillet->modifierDirection(false,DIR_HAUT);

	if(touche == ConfigScene::obtenirInstance()->obtenirToucheGauche())
		maillet->modifierDirection(false,DIR_GAUCHE);

	if(touche == ConfigScene::obtenirInstance()->obtenirToucheDroite())
		maillet->modifierDirection(false,DIR_DROITE);

	if(touche == ConfigScene::obtenirInstance()->obtenirToucheBas())
		maillet->modifierDirection(false,DIR_BAS);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeTournoi::sourisEnfoncee(EvenementSouris& evenementSouris)
///
/// Fonction qui g�re les �v�nements li�s � l'enfoncement d'un bouton de la souris.
///
/// @param[in] evenementClavier : L'objet correspondant � l'�v�nement.
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
/// Fonction qui g�re les �v�nements li�s au rel�chement d'un bouton de la souris.
///
/// @param[in] evenementClavier : L'objet correspondant � l'�v�nement.
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
/// Fonction qui g�re les �v�nements li�s au d�placement d'un bouton de la souris.
///
/// @param[in] evenementSouris : L'objet correspondant � l'�v�nement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeTournoi::sourisDeplacee( EvenementSouris& evenementSouris )
{
	Vecteur3 coordonneesSouris, anciennePos;
	FacadeModele::getInstance()->convertirClotureAVirtuelle(evenementSouris.obtenirPosition()[VX], evenementSouris.obtenirPosition()[VY], coordonneesSouris);

    auto game = FacadeModele::getInstance()->obtenirPartieCourante();
    checkf(game);
    if(game)
    {
        NoeudMaillet* mailletGauche = game->getField()->getLeftMallet();
        NoeudMaillet* mailletDroit = game->getField()->getRightMallet();
        checkf(mailletGauche && mailletDroit);
        if(mailletGauche && mailletDroit)
        {
            if(!mailletGauche->estControleParNetwork())
                mailletGauche->setTargetDestination(coordonneesSouris);
            if(!mailletDroit->estControleParNetwork())
                mailletDroit->setTargetDestination(coordonneesSouris);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeTournoi::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
///
/// Fonction qui g�re les �v�nements li�s au d�placement de la roulette de la souris
///
/// @param[in] evenementRouletteSouris : L'objet correspondant � l'�v�nement.
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
		if(!FacadeModele::getInstance()->estEnPause() && !GestionnaireAnimations::obtenirInstance()->estJouerReplay())
		{
			Tournoi* tournoi = FacadeModele::getInstance()->obtenirTournoi();
			if(tournoi->miseAJour())
			{
				GestionnaireEvenements::modifierEtat(ETAT_PARTIE_TOURNOI_TERMINEE);
				return;
			}
            partieCourante->animer(temps);
		}
	}

	gestionAnimationEnJeu(partieCourante, temps);

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
        vue::Camera ancienneCamera = GestionnaireAnimations::obtenirInstance()->obtenirAncienneCamera();
        vue::Camera* camera = &FacadeModele::getInstance()->obtenirVue()->obtenirCamera();

        AnimationFrame* frame[2];
        frame[0] = new AnimationFrame(0, camera->obtenirPosition(), camera->obtenirPointVise(), camera->obtenirDirectionHaut());
        frame[1] = new AnimationFrame(500, ancienneCamera.obtenirPosition(), ancienneCamera.obtenirPointVise(), ancienneCamera.obtenirDirectionHaut());

        Animation* animation = new Animation(BEZIER, true, true, true);
        for(int i=0; i<2; i++)
            animation->ajouterFrame(frame[i]);
        animation->ajouterObjet(camera);
        GestionnaireAnimations::obtenirInstance()->ajouterAnimation(animation);

        Partie* partieCourante = FacadeModele::getInstance()->obtenirPartieCourante();
        if(partieCourante)
            partieCourante->delais(4100);
    }
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
