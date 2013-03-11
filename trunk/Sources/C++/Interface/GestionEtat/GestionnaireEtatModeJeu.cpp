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
#include "NoeudMaillet.h"
#include "ConfigScene.h"
#include "GestionnaireAnimations.h"
#include "Partie.h"
#include "Terrain.h"
#include "GestionnaireHUD.h"
#include "GestionnaireEtatAbstrait.h"
#include "SourisEtatAbstrait.h"
#include "SoundFMOD.h"




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
GestionnaireEtatModeJeu::GestionnaireEtatModeJeu( Partie* pGame ):
GestionnaireEtatAbstrait(),mGame(pGame)
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
    checkf(mGame);
    if(mGame)
    {
        NoeudMaillet* maillet = mGame->getField()->getRightMallet();
        checkf(maillet);
        if(maillet)
        {
            // Les 4 cas suivants déplacent le maillet du joueur 2
            if(touche == ConfigScene::obtenirInstance()->obtenirToucheHaut())
                maillet->modifierDirection(true,DIR_HAUT);

            if(touche == ConfigScene::obtenirInstance()->obtenirToucheGauche())
                maillet->modifierDirection(true,DIR_GAUCHE);

            if(touche == ConfigScene::obtenirInstance()->obtenirToucheDroite())
                maillet->modifierDirection(true,DIR_DROITE);

            if(touche == ConfigScene::obtenirInstance()->obtenirToucheBas())
                maillet->modifierDirection(true,DIR_BAS);
        }
    }

	
	// VERIF ETAT PAUSE
	if(touche==VJAK_SHIFT)
		shiftEnfonce_ = true;
	else
		toucheSauvegardee_ = touche;

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


    checkf(mGame);
    if(mGame)
    {
        NoeudMaillet* maillet = mGame->getField()->getRightMallet();
        checkf(maillet);
        if(maillet)
        {
            // Les 4 cas suivants déplacent le maillet du joueur 2
            if(touche == ConfigScene::obtenirInstance()->obtenirToucheHaut())
                maillet->modifierDirection(false,DIR_HAUT);

            if(touche == ConfigScene::obtenirInstance()->obtenirToucheGauche())
                maillet->modifierDirection(false,DIR_GAUCHE);

            if(touche == ConfigScene::obtenirInstance()->obtenirToucheDroite())
                maillet->modifierDirection(false,DIR_DROITE);

            if(touche == ConfigScene::obtenirInstance()->obtenirToucheBas())
                maillet->modifierDirection(false,DIR_BAS);
        }
    }
	
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
	else
	{
        checkf(mGame);
        if(mGame)
        {
            NoeudMaillet* mailletGauche = mGame->getField()->getLeftMallet();
            NoeudMaillet* mailletDroit = mGame->getField()->getRightMallet();
            checkf(mailletGauche && mailletDroit);
            if(mailletGauche && mailletDroit)
            {
                mailletGauche->assignerPosSouris(coordonneesSouris);
                mailletDroit->assignerPosSouris(coordonneesSouris);	
            }
        }
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
	Partie* partieCourante = mGame;

	if(partieCourante)
	{
		partieCourante->animer(temps);
		partieCourante->updateMinuterie((int)(temps*1000));
        if(partieCourante->partieTerminee())
        {
            GestionnaireEvenements::modifierEtat(ETAT_PARTIE_RAPIDE_TERMINEE);
            return;
        }
	}
	gestionAnimationEnJeu(partieCourante, temps);
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
    if(mGame)
    {
        renderBase(mGame->getField(),[&]() -> void{mGame->afficher();});
    }
    GestionnaireHUD::obtenirInstance()->dessinerHUDJeu();
}

void GestionnaireEtatModeJeu::miseAJourEvenementsRepetitifs( float deltaTemps )
{
	int tempsMs = (int)deltaTemps*1000;

	// VERIF ETAT SOURIS
	switch(toucheSauvegardee_)
	{
	case VJAK_UP:
		FacadeModele::getInstance()->deplacerFleches(Vecteur2i(0, -tempsMs));
		break;
	case VJAK_DOWN:
		FacadeModele::getInstance()->deplacerFleches(Vecteur2i(0, tempsMs));
		break;
	case VJAK_LEFT:
		FacadeModele::getInstance()->deplacerFleches(Vecteur2i(-tempsMs, 0));
		break;
	case VJAK_RIGHT:
		FacadeModele::getInstance()->deplacerFleches(Vecteur2i(tempsMs, 0));
		break;
	case VJAK_ADD:
	case VJAK_PLUS:
		// Utilisation temporaire de la méthode pour le zooom associé à la roulette de la souris
		// -1 indique que c'est un zoomIn
		FacadeModele::getInstance()->zoom(-tempsMs);
		break;

	case VJAK_SUBTRACT:
	case VJAK_MINUS:
		// Utilisation temporaire de la méthode pour le zooom associé à la roulette de la souris
		// 1 indique que c'est un zoomOut
		FacadeModele::getInstance()->zoom(tempsMs);
		break;

	default:
		break;
	}


}









///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




