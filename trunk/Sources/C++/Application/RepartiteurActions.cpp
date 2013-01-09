///////////////////////////////////////////////////////////////////////////////
/// @file RepartiteurActions.cpp
/// @author Michael Ferris
/// @date 2012-03-09
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "RepartiteurActions.h"
#include "FacadeModele.h"
#include "GestionnaireEvenements.h"
#include "VisiteurSuppression.h"
#include "GestionnaireEtatAbstrait.h"
#include "ArbreRenduINF2990.h"
#include "VisiteurDupliquer.h"
#include "GestionnaireAnimations.h"
#include "VuePerspectiveCiel.h"
#include "VuePerspectiveOrbit.h"
#include "VuePerspectiveLibre.h"
#include "Camera.h"
#include "SoundFMOD.h"
#include "VuePerspectiveSplit.h"
#include "Partie.h"
#include "ObjetAnimable.h"

// Initialisations automatiques
SINGLETON_DECLARATION_CPP(RepartiteurActions);

////////////////////////////////////////////////////////////////////////
///
/// @fn  RepartiteurActions::RepartiteurActions(  )
///
/// Constructeur, initialise la banque contenant la reference sur les action concernes
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
RepartiteurActions::RepartiteurActions()
{
	banqueActions_["EDITEUR_NOUVEAU"] = &RepartiteurActions::actionBoutonEditionNouveau;
	banqueActions_["REINITIALISER_PARTIE"] = &RepartiteurActions::actionReinitialiserPartie;
	banqueActions_["PAUSE_JEU"] = &RepartiteurActions::actionTogglePauseJeu;
	banqueActions_["REPLAY"] = &RepartiteurActions::actionReplay;

	// Fonctions de zoom
	banqueActions_["CAMERA"] = &RepartiteurActions::actionBoutonCamera;
	banqueActions_["ORBIT"] = &RepartiteurActions::actionBoutonOrbit;
	banqueActions_["ZOOM_ELASTIQUE"] = &RepartiteurActions::actionBoutonZoomElastique;
	banqueActions_["ZOOM_PROPORTIONNEL"] = &RepartiteurActions::actionBoutonZoomProportionnel;

	// Fonctions d'�dition
	banqueActions_["EDITEUR_SELECTION"] = &RepartiteurActions::actionBoutonSelection;
	banqueActions_["EDITEUR_DEPLACER"] = &RepartiteurActions::actionBoutonTransformationDeplacement;
	banqueActions_["EDITEUR_ROTATION"] = &RepartiteurActions::actionBoutonTransformationRotation;
	banqueActions_["EDITEUR_ECHELLE"] = &RepartiteurActions::actionBoutonTransformationEchelle;
	banqueActions_["SUPPRIMER"] = &RepartiteurActions::actionBoutonSupprimer;
	banqueActions_["DUPLIQUER"] = &RepartiteurActions::actionBoutonDupliquer;

	// Fonctions d'insertions.
	banqueActions_["INSERER_PORTAIL"] = &RepartiteurActions::actionBoutonInsererPortail;
	banqueActions_["INSERER_MURET"] = &RepartiteurActions::actionBoutonInsererMuret;
	banqueActions_["INSERER_MAILLET"] = &RepartiteurActions::actionBoutonInsererMaillet;
	banqueActions_["INSERER_RONDELLE"] = &RepartiteurActions::actionBoutonInsererRondelle;
	banqueActions_["INSERER_ACCELERATEUR"] = &RepartiteurActions::actionBoutonInsererAccelerateur;

	// Fonctions de changement de mode
	banqueActions_["ALLER_MODE_EDITION"] = &RepartiteurActions::actionBoutonAllerModeEdition;
	banqueActions_["ALLER_MODE_JEU"] = &RepartiteurActions::actionBoutonAllerModeJeu;
	//banqueActions_["ALLER_MODE_TOURNOI"] = &RepartiteurActions::actionBoutonAllerModeTournoi;

	// Fonctions de changement de camera
	banqueActions_["CAMERA_FIXE"] = &RepartiteurActions::actionChangerModeCameraFixe;
	banqueActions_["CAMERA_ORBITE"] = &RepartiteurActions::actionChangerModeCameraOrbite;
	banqueActions_["CAMERA_LIBRE"] = &RepartiteurActions::actionChangerModeCameraLibre;
	banqueActions_["CAMERA_SPLIT"] = &RepartiteurActions::actionChangerModeCameraSplit;

}


////////////////////////////////////////////////////////////////////////
///
/// @fn  RepartiteurActions::~RepartiteurActions(  )
///
/// Destructeur Vide
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
RepartiteurActions::~RepartiteurActions()
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::appelerMethodeAction( std::string& action )
///
/// Fonction qui appelle les fonctions de la banque d'actions
///
/// @param[in] std::string & action
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::appelerMethodeAction( std::string& action )
{
	if(banqueActions_.find(action) != banqueActions_.end())
		(this ->*(banqueActions_[action]))();
	return true;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonCamera()
///
/// Fonction d'action pour changer l'�tat de la souris sur D�placer Fen�tre
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonCamera()
{
	GestionnaireEvenements::obtenirInstance()->obtenirEtat()->modifierEtatSouris(ETAT_SOURIS_DEPLACER_FENETRE);
	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void RepartiteurActions::actionBoutonOrbit()
///
/// Fonction d'action pour changer l'�tat de la souris sur tourner la camera Fen�tre
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonOrbit()
{
	GestionnaireEvenements::obtenirInstance()->obtenirEtat()->modifierEtatSouris(ETAT_SOURIS_ORBIT);
	return true;
}



////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonZoomElastique()
///
/// Fonction d'Action pour changer l'�tat de la souris sur Zoom �lastique
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonZoomElastique()
{
	GestionnaireEvenements::obtenirInstance()->obtenirEtat()->modifierEtatSouris(ETAT_SOURIS_ZOOM_ELASTIQUE);
	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonZoomProportionnel()
///
/// Fonction d'Action pour changer l'�tat de la souris sur Zoom Proportionnel
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonZoomProportionnel()
{
	GestionnaireEvenements::obtenirInstance()->obtenirEtat()->modifierEtatSouris(ETAT_SOURIS_ZOOM_PROPORTIONNEL);
	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonSelection()
///
/// Fonction d'action pour g�rer la s�lection
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonSelection()
{
	GestionnaireEvenements::obtenirInstance()->obtenirEtat()->modifierEtatSouris(ETAT_SOURIS_SELECTION);
	return true; 
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonTransformationDeplacement()
///
/// Fonction d'action pour g�rer les d�placements
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonTransformationDeplacement()
{
	GestionnaireEvenements::obtenirInstance()->obtenirEtat()->modifierEtatSouris(ETAT_SOURIS_TRANSFORMATION_DEPLACEMENT);
	return true; 
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonTransformationRotation()
///
/// Fonction d'Action pour changer l'�tat de la souris sur Trasformation Rotation
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonTransformationRotation()
{
	GestionnaireEvenements::obtenirInstance()->obtenirEtat()->modifierEtatSouris(ETAT_SOURIS_TRANSFORMATION_ROTATION);
	return true; 
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonTransformationEchelle()
///
/// Fonction d'Action pour changer l'�tat de la souris sur Trasformation �chelle
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonTransformationEchelle()
{
	GestionnaireEvenements::obtenirInstance()->obtenirEtat()->modifierEtatSouris(ETAT_SOURIS_TRANSFORMATION_ECHELLE);
	return true; 
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonSupprimer()
///
/// Fonction d'action pour supprimer la s�lection actuelle.
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonSupprimer()
{
	FacadeModele::obtenirInstance()->visiterArbre(&VisiteurSuppression());
	return true; 
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonEditionNouveau()
///
/// Fonction d'action de cr�ation d'un nouveau fichier d'edition.
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonEditionNouveau()
{
	GestionnaireEvenements::obtenirInstance()->obtenirEtat()->modifierEtatSouris(ETAT_SOURIS_ORBIT);
	FacadeModele::obtenirInstance()->reinitialiserTerrain();	
	return true; 
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionReinitialiserPartie(  )
///
/// Fonction d'action pour reinitialisaer une partie
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionReinitialiserPartie()
{
	FacadeModele::obtenirInstance()->reinitialiserPartie();
	return true; 
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonInsererPortail()
///
/// Fonction d'action pour inserer un portail dans l'espace de jeu.
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonInsererPortail()
{
	FacadeModele::obtenirInstance()->obtenirArbreRenduINF2990()->deselectionnerTout();
	GestionnaireEvenements::obtenirInstance()->obtenirEtat()->modifierEtatSouris(ETAT_SOURIS_AJOUTER_PORTAIL);
	return true; 
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonInsererMuret()
///
/// Fonction d'action pour inserer un muret dans l'espace de jeu.
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonInsererMuret()
{
	FacadeModele::obtenirInstance()->obtenirArbreRenduINF2990()->deselectionnerTout();
	GestionnaireEvenements::obtenirInstance()->obtenirEtat()->modifierEtatSouris(ETAT_SOURIS_AJOUTER_MURET);
	return true; 
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonInsererMaillet()
///
/// Fonction d'action pour inserer un maillet dans l'espace de jeu.
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonInsererMaillet()
{
	FacadeModele::obtenirInstance()->obtenirArbreRenduINF2990()->deselectionnerTout();
	GestionnaireEvenements::obtenirInstance()->obtenirEtat()->modifierEtatSouris(ETAT_SOURIS_AJOUTER_MAILLET);
	return true; 
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonInsererRondelle()
///
/// Fonction pour inserer les rondelles.
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonInsererRondelle()
{
	FacadeModele::obtenirInstance()->obtenirArbreRenduINF2990()->deselectionnerTout();
	GestionnaireEvenements::obtenirInstance()->obtenirEtat()->modifierEtatSouris(ETAT_SOURIS_AJOUTER_RONDELLE);
	return true; 
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonInsererAccelerateur()
///
/// Fonction d'action pour inserer un accelerateur.
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonInsererAccelerateur()
{
	FacadeModele::obtenirInstance()->obtenirArbreRenduINF2990()->deselectionnerTout();
	GestionnaireEvenements::obtenirInstance()->obtenirEtat()->modifierEtatSouris(ETAT_SOURIS_AJOUTER_ACCELERATEUR);
	return true; 
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonDupliquer()
///
/// Fonction d'action pour dupliquer
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonDupliquer()
{
	FacadeModele::obtenirInstance()->visiterArbre(&VisiteurDupliquer(FacadeModele::obtenirInstance()->obtenirArbreRenduINF2990()));
	return true; 
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonAllerModeJeu()
///
/// Fonction pour aller en mode jeu
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonAllerModeJeu()
{
	bool retour = FacadeModele::obtenirInstance()->passageModeJeu();
	if(retour)
		GestionnaireEvenements::obtenirInstance()->modifierEtat(ETAT_MODE_JEU);
	return retour;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonAllerModeEdition()
///
/// Fonction pour aller en mode �dition
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonAllerModeEdition()
{
	bool retour =  FacadeModele::obtenirInstance()->passageModeEdition();
	if(retour)
	{
		GestionnaireEvenements::obtenirInstance()->modifierEtat(ETAT_MODE_EDITION);
		SoundFMOD::obtenirInstance()->playApplicationSong(EDITION_MODE_SONG);
	}
	return retour;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonAllerModeTournoi()
///
/// Fonction pour aller en mode tournoi
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonAllerModeTournoi()
{
	bool retour =  FacadeModele::obtenirInstance()->passageModeTournoi();
	if(retour)
	{
		GestionnaireEvenements::obtenirInstance()->modifierEtat(ETAT_MODE_TOURNOI);
	}
	return retour;
}



////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonAllerMenuPrincipal()
///
/// Passage au mode menu principal.
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonAllerMenuPrincipal()
{
	SoundFMOD::obtenirInstance()->playApplicationSong(MENU_MODE_SONG);
	GestionnaireEvenements::obtenirInstance()->modifierEtat(ETAT_MENU_PRINCIPAL);
	return FacadeModele::obtenirInstance()->passageMenuPrincipal();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionTogglePauseJeu()
///
/// Activation ou d�sactivation du mode pause.
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionTogglePauseJeu()
{
	FacadeModele::obtenirInstance()->togglePause();
	SoundFMOD::obtenirInstance()->playEffect(effect(PAUSE_EFFECT));
	return true; 
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionReplay()
///
/// Permet de jouer un replay
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionReplay()
{
	// Toggle
	if(GestionnaireAnimations::obtenirInstance()->estJouerReplay())
	{
		vue::Vue* vueCourante = FacadeModele::obtenirInstance()->obtenirVue();
		vue::Camera* camera = &vueCourante->obtenirCamera();
		vue::Camera ancienneCamera = GestionnaireAnimations::obtenirInstance()->obtenirAncienneCamera();

		AnimationFrame* frame[2];
		frame[0] = new AnimationFrame(0, camera->obtenirPosition(), camera->obtenirPointVise(), camera->obtenirDirectionHaut());
		frame[1] = new AnimationFrame(500, ancienneCamera.obtenirPosition(), ancienneCamera.obtenirPointVise(), ancienneCamera.obtenirDirectionHaut());

		Animation* animation = new Animation(BEZIER, true, true, true);
		for(int i=0; i<2; i++)
			animation->ajouterFrame(frame[i]);
		animation->ajouterObjet((ObjetAnimable*)&(vueCourante->obtenirCamera()));
		GestionnaireAnimations::obtenirInstance()->ajouterAnimation(animation);

		GestionnaireAnimations::obtenirInstance()->terminerReplay();
		Partie* partie = FacadeModele::obtenirInstance()->obtenirPartieCourante();
		if(partie && !partie->partieTerminee())
			partie->obtenirGameTime()->unPause();
	}
	else
	{
		vue::Vue* vueCourante = FacadeModele::obtenirInstance()->obtenirVue();
		vue::Camera* camera = &vueCourante->obtenirCamera();
		
		vue::Camera ancienneCamera = GestionnaireAnimations::obtenirInstance()->obtenirAncienneCamera();
		// Mettre quelque chose du genre pour effectuer un fondu au debut du replay
		/*AnimationFrame* frame[2];
		frame[0] = new AnimationFrame(0, camera->obtenirPosition(), camera->obtenirPointVise(), camera->obtenirDirectionHaut());
		frame[1] = new AnimationFrame(500, ancienneCamera.obtenirPosition(), ancienneCamera.obtenirPointVise(), ancienneCamera.obtenirDirectionHaut());

		Animation* animation = new Animation(BEZIER, true, true, true);
		for(int i=0; i<2; i++)
			animation->ajouterFrame(frame[i]);
		animation->ajouterObjet((ObjetAnimable*)&(vueCourante->obtenirCamera()));
		GestionnaireAnimations::obtenirInstance()->ajouterAnimation(animation);*/
		FacadeModele::obtenirInstance()->obtenirPartieCourante()->obtenirGameTime()->pause();
		GestionnaireAnimations::obtenirInstance()->jouerReplay(FacadeModele::obtenirInstance()->obtenirVue()->obtenirCamera());
	}
	return true; 
}



////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionChangerModeCameraFixe()
///
/// Permet de changer le mode de camera
/// 
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionChangerModeCameraFixe()
{
	int xMinCourant, yMinCourant, xMaxCourant, yMaxCourant;
	FacadeModele::obtenirInstance()->obtenirVue()->obtenirProjection().obtenirCoordonneesCloture(xMinCourant, xMaxCourant, yMinCourant, yMaxCourant);

	vue::VuePerspectiveCiel* nouvelleVue = new vue::VuePerspectiveCiel(
		vue::Camera(Vecteur3(0, -50, 300), Vecteur3(0, 0, 0),
		Vecteur3(0, 1, 0)  , Vecteur3(0, 1, 0)),
		0, 400, 0, 400,
		180, 50000, /*ZoomInMax*/10, /*ZoomOutMax*/15000, 1.25,
		-150, 150, -150, 150);
	FacadeModele::obtenirInstance()->obtenirVue()->centrerCamera(FacadeModele::obtenirInstance()->obtenirLargeurZoneEdition());
	nouvelleVue->redimensionnerFenetre(Vecteur2i(xMinCourant, yMinCourant), Vecteur2i(xMaxCourant, yMaxCourant));


	FacadeModele::obtenirInstance()->modifierVue(nouvelleVue);

	return true; 
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionChangerModeCameraOrbite()
///
/// Permet de changer le mode de camera
/// 
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionChangerModeCameraOrbite()
{
	vue::Camera cameraCourante = FacadeModele::obtenirInstance()->obtenirVue()->obtenirCamera();
	int xMinCourant, yMinCourant, xMaxCourant, yMaxCourant;
	FacadeModele::obtenirInstance()->obtenirVue()->obtenirProjection().obtenirCoordonneesCloture(xMinCourant, xMaxCourant, yMinCourant, yMaxCourant);

	vue::VuePerspectiveOrbit* nouvelleVue = new vue::VuePerspectiveOrbit(
		cameraCourante,
		0, 400, 0, 400,
		180, 50000, /*ZoomInMax*/10, /*ZoomOutMax*/15000, 1.25,
		-150, 150, -150, 150);
	FacadeModele::obtenirInstance()->obtenirVue()->centrerCamera(FacadeModele::obtenirInstance()->obtenirLargeurZoneEdition());
	nouvelleVue->redimensionnerFenetre(Vecteur2i(xMinCourant, yMinCourant), Vecteur2i(xMaxCourant, yMaxCourant));

	FacadeModele::obtenirInstance()->modifierVue(nouvelleVue);

	return true; 
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionChangerModeCameraLibre()
///
/// Permet de changer le mode de camera
/// 
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionChangerModeCameraLibre()
{
	vue::Camera cameraCourante = FacadeModele::obtenirInstance()->obtenirVue()->obtenirCamera();
	int xMinCourant, yMinCourant, xMaxCourant, yMaxCourant;
	FacadeModele::obtenirInstance()->obtenirVue()->obtenirProjection().obtenirCoordonneesCloture(xMinCourant, xMaxCourant, yMinCourant, yMaxCourant);


	vue::VuePerspectiveLibre* nouvelleVue = new vue::VuePerspectiveLibre(
		cameraCourante,
		0, 300, 0, 400,
		180, 50000, /*ZoomInMax*/10, /*ZoomOutMax*/15000, 1.25,
		-150, 150, -150, 150);
	FacadeModele::obtenirInstance()->obtenirVue()->centrerCamera(FacadeModele::obtenirInstance()->obtenirLargeurZoneEdition());
	nouvelleVue->redimensionnerFenetre(Vecteur2i(xMinCourant, yMinCourant), Vecteur2i(xMaxCourant, yMaxCourant));

	FacadeModele::obtenirInstance()->modifierVue(nouvelleVue);
	return true; 
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionChangerModeCameraSplit()
///
/// Permet de changer le mode de camera
/// 
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionChangerModeCameraSplit()
{
	int xMinCourant, yMinCourant, xMaxCourant, yMaxCourant;
	FacadeModele::obtenirInstance()->obtenirVue()->obtenirProjection().obtenirCoordonneesCloture(xMinCourant, xMaxCourant, yMinCourant, yMaxCourant);

	vue::Vue* nouvelleVue = new vue::VuePerspectiveSplit(
		vue::Camera(Vecteur3(-200.0, 0.0, 75.0), Vecteur3(0, 0, 0),
		Vecteur3(0, 0, 1)  , Vecteur3(0, 1, 0)),
		0, 400, 0, 400,
		180, 50000, /*ZoomInMax*/10, /*ZoomOutMax*/15000, 1.25,
		-150, 150, -150, 150);
	FacadeModele::obtenirInstance()->obtenirVue()->centrerCamera(FacadeModele::obtenirInstance()->obtenirLargeurZoneEdition());
	nouvelleVue->redimensionnerFenetre(Vecteur2i(xMinCourant, yMinCourant), Vecteur2i(xMaxCourant, yMaxCourant));


	FacadeModele::obtenirInstance()->modifierVue(nouvelleVue);

	return true; 
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////