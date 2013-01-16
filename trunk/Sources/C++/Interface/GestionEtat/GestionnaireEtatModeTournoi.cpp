//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEtatModeTournoi.cpp
/// @author Vincent Lemire
/// @date 2012-03-02
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
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
GestionnaireEtatModeTournoi::GestionnaireEtatModeTournoi(GestionnaireEvenements* contexte) : GestionnaireEtatAbstrait(contexte)
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
	NoeudMaillet* maillet = FacadeModele::getInstance()->obtenirMailletJoueurDroit();

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
	ToucheClavier touche = evenementClavier.obtenirTouche();
	NoeudMaillet* maillet = FacadeModele::getInstance()->obtenirMailletJoueurDroit();

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
	Vecteur3 coordonneesSouris, anciennePos;
	FacadeModele::getInstance()->convertirClotureAVirtuelle(evenementSouris.obtenirPosition()[VX], evenementSouris.obtenirPosition()[VY], coordonneesSouris);
	NoeudMaillet* maillet = FacadeModele::getInstance()->obtenirMailletJoueurGauche();
	maillet->assignerPosSouris(coordonneesSouris);
	maillet = FacadeModele::getInstance()->obtenirMailletJoueurDroit();
	maillet->assignerPosSouris(coordonneesSouris);	
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
		if(!FacadeModele::getInstance()->estEnPause() && !GestionnaireAnimations::obtenirInstance()->estJouerReplay())
		{
			Tournoi* tournoi = FacadeModele::getInstance()->obtenirTournoi();
			if(tournoi->miseAJour())
			{
				contexte_->modifierEtat(ETAT_PARTIE_TOURNOI_TERMINEE);
				return;
			}
			Terrain* terrain = FacadeModele::getInstance()->getTerrain();
			if(partieCourante->estPret() && terrain)
			{
				// Gestion de la physique du jeu
				terrain->appliquerPhysique(temps);
			}

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
	if(tournoi && tournoi->obtenirPartieCourante() && tournoi->obtenirPartieCourante()->partieTerminee())
	{
		GestionnaireHUD::obtenirInstance()->dessinerHUDTournoi();
	}
	else
	{
		GestionnaireHUD::obtenirInstance()->dessinerHUDJeu();
	}
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
