///////////////////////////////////////////////////////////////////////////////
/// @file RepartiteurActions.cpp
/// @author Michael Ferris
/// @date 2012-03-09
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "RepartiteurActions.h"
#include "FacadeModele.h"
#include "EventManager.h"
#include "GameControllerAbstract.h"

#include "VuePerspectiveCiel.h"
#include "VuePerspectiveOrbit.h"
#include "VuePerspectiveLibre.h"
#include "Camera.h"
#include "SoundFMOD.h"
#include "VuePerspectiveSplit.h"
#include "Partie.h"
#include "Terrain.h"
#include "..\Reseau\GestionnaireReseau.h"
#include "..\Reseau\Paquets\PaquetGameEvent.h"
#include "..\Reseau\RelayeurMessage.h"

// Initialisations automatiques
SINGLETON_DECLARATION_CPP(RepartiteurActions);




///////////////////////////////////////////////////////////////////
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
    banqueActions_[ACTION_EDITEUR_NOUVEAU]       = &RepartiteurActions::actionBoutonEditionNouveau;
    banqueActions_[ACTION_REINITIALISER_PARTIE]  = &RepartiteurActions::actionReinitialiserPartie;
    banqueActions_[ACTION_REINITIALISER_RONDELLE]  = &RepartiteurActions::actionReinitialiserRondelle;
    banqueActions_[ACTION_PAUSE_JEU]             = &RepartiteurActions::actionTogglePauseJeu;
    banqueActions_[ACTION_REPLAY]                = &RepartiteurActions::actionReplay;
    
    //Fonctions de zoom
    banqueActions_[ACTION_CAMERA]                = &RepartiteurActions::actionBoutonCamera;
    banqueActions_[ACTION_ORBIT]                 = &RepartiteurActions::actionBoutonOrbit;
    banqueActions_[ACTION_ZOOM_ELASTIQUE]        = &RepartiteurActions::actionBoutonZoomElastique;
    banqueActions_[ACTION_ZOOM_PROPORTIONNEL]    = &RepartiteurActions::actionBoutonZoomProportionnel;

    //Fonctions d'�dition
    banqueActions_[ACTION_EDITEUR_SELECTION]     = &RepartiteurActions::actionBoutonSelection;
    banqueActions_[ACTION_EDITEUR_DEPLACER]      = &RepartiteurActions::actionBoutonTransformationDeplacement;
    banqueActions_[ACTION_EDITEUR_ROTATION]      = &RepartiteurActions::actionBoutonTransformationRotation;
    banqueActions_[ACTION_EDITEUR_ECHELLE]       = &RepartiteurActions::actionBoutonTransformationEchelle;
    banqueActions_[ACTION_EDITEUR_UNDO]          = &RepartiteurActions::actionBoutonUndo;
    banqueActions_[ACTION_EDITEUR_REDO]          = &RepartiteurActions::actionBoutonRedo;
    banqueActions_[ACTION_SUPPRIMER]             = &RepartiteurActions::actionBoutonSupprimer;
    banqueActions_[ACTION_DUPLIQUER]             = &RepartiteurActions::actionBoutonDupliquer;
    banqueActions_[ACTION_PIE_MODE]          = &RepartiteurActions::actionPlayInEditor;

    //Fonctions d'insertions.
    banqueActions_[ACTION_INSERER_PORTAIL]       = &RepartiteurActions::actionBoutonInsererPortail;
    banqueActions_[ACTION_INSERER_MURET]         = &RepartiteurActions::actionBoutonInsererMuret;
    banqueActions_[ACTION_INSERER_MAILLET]       = &RepartiteurActions::actionBoutonInsererMaillet;
    banqueActions_[ACTION_INSERER_RONDELLE]      = &RepartiteurActions::actionBoutonInsererRondelle;
    banqueActions_[ACTION_INSERER_ACCELERATEUR]  = &RepartiteurActions::actionBoutonInsererAccelerateur;
    banqueActions_[ACTION_INSERER_BONUS]         = &RepartiteurActions::actionBoutonInsererBonus;

    //Fonctions de changement de mode
    banqueActions_[ACTION_ALLER_MODE_EDITION]    = &RepartiteurActions::actionBoutonAllerModeEdition;
    banqueActions_[ACTION_ALLER_MODE_EDITION_ONLINE]    = &RepartiteurActions::actionBoutonAllerModeEditionOnline;
    banqueActions_[ACTION_ALLER_MODE_JEU]        = &RepartiteurActions::actionBoutonAllerModeJeu;
    banqueActions_[ACTION_ALLER_MODE_TOURNOI]    = &RepartiteurActions::actionBoutonAllerModeTournoi;
    banqueActions_[ACTION_ALLER_MENU_PRINCIPAL]  = &RepartiteurActions::actionBoutonAllerMenuPrincipal;
    banqueActions_[ACTION_ALLER_MODE_SIMULATION] = &RepartiteurActions::actionBoutonAllerModeSimulation;
    banqueActions_[ACTION_ALLER_MODE_GOL]        = &RepartiteurActions::actionBoutonAllerGameOfLife;

    //Fonctions de changement de camera
    banqueActions_[ACTION_CAMERA_FIXE]           = &RepartiteurActions::actionChangerModeCameraFixe;
    banqueActions_[ACTION_CAMERA_ORBITE]         = &RepartiteurActions::actionChangerModeCameraOrbite;
    banqueActions_[ACTION_CAMERA_LIBRE]          = &RepartiteurActions::actionChangerModeCameraLibre;
    banqueActions_[ACTION_CAMERA_SPLIT]          = &RepartiteurActions::actionChangerModeCameraSplit;

    

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
bool RepartiteurActions::appelerMethodeAction( ActionType action )
{
    if(banqueActions_.find(action) != banqueActions_.end())
        return (this ->*(banqueActions_[action]))();
    return false;
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
    EventManager::modifierEtatSouris(ETAT_SOURIS_DEPLACER_FENETRE);
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
    EventManager::modifierEtatSouris(ETAT_SOURIS_ORBIT);
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
    EventManager::modifierEtatSouris(ETAT_SOURIS_ZOOM_ELASTIQUE);
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
    EventManager::modifierEtatSouris(ETAT_SOURIS_ZOOM_PROPORTIONNEL);
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
    EventManager::modifierEtatSouris(ETAT_SOURIS_SELECTION);
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
    EventManager::modifierEtatSouris(ETAT_SOURIS_TRANSFORMATION_DEPLACEMENT);
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
    EventManager::modifierEtatSouris(ETAT_SOURIS_TRANSFORMATION_ROTATION);
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
    EventManager::modifierEtatSouris(ETAT_SOURIS_TRANSFORMATION_ECHELLE);
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
    FacadeModele::getInstance()->getEditionField()->deleteSelectedNodes();
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
    EventManager::modifierEtatSouris(ETAT_SOURIS_ORBIT);
    FacadeModele::getInstance()->reinitialiserTerrain();	
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
    FacadeModele::getInstance()->reinitialiserPartie();
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
    FacadeModele::getInstance()->selectionArbre(false);
    EventManager::modifierEtatSouris(ETAT_SOURIS_AJOUTER_PORTAIL);
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
    FacadeModele::getInstance()->selectionArbre(false);
    EventManager::modifierEtatSouris(ETAT_SOURIS_AJOUTER_MURET);
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
    FacadeModele::getInstance()->selectionArbre(false);
    EventManager::modifierEtatSouris(ETAT_SOURIS_AJOUTER_MAILLET);
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
    FacadeModele::getInstance()->selectionArbre(false);
    EventManager::modifierEtatSouris(ETAT_SOURIS_AJOUTER_RONDELLE);
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
    FacadeModele::getInstance()->selectionArbre(false);
    EventManager::modifierEtatSouris(ETAT_SOURIS_AJOUTER_ACCELERATEUR);
    return true; 
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonInsererBonus()
///
///Fonction d'action pour inserer un bonus.
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonInsererBonus()
{
    FacadeModele::getInstance()->selectionArbre(false);
    EventManager::modifierEtatSouris(ETAT_SOURIS_AJOUTER_BONUS);
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
    FacadeModele::getInstance()->getEditionField()->duplicateSelection();
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
    // important de signaler le modele avant de faire le changement d'�tat, car
    // celui-ci utilise des informations du mod�le pour s'initialiser
    bool retour = FacadeModele::getInstance( )->changeControler( GAME_CONTROLLER_GAME );
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
    // important de signaler le modele avant de faire le changement d'�tat, car
    // celui-ci utilise des informations du mod�le pour s'initialiser
    bool retour = FacadeModele::getInstance( )->changeControler( GAME_CONTROLLER_EDITION_LOAD_DEFAULT);
    return retour;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonAllerModeEditionPasDefautXML()
///
/// Fonction pour aller en mode �dition
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonAllerModeEditionOnline()
{
    // important de signaler le modele avant de faire le changement d'�tat, car
    // celui-ci utilise des informations du mod�le pour s'initialiser
    bool retour = FacadeModele::getInstance( )->changeControler( GAME_CONTROLLER_EDITION );
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
    // important de signaler le modele avant de faire le changement d'�tat, car
    // celui-ci utilise des informations du mod�le pour s'initialiser
    bool retour = FacadeModele::getInstance( )->changeControler( GAME_CONTROLLER_TOURNAMENT );
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
    // important de signaler le modele avant de faire le changement d'�tat, car
    // celui-ci utilise des informations du mod�le pour s'initialiser
    bool retour = FacadeModele::getInstance( )->changeControler( GAME_CONTROLLER_MENU );
    return retour;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonAllerMenuPrincipal()
///
///Fonction pour aller au mode simulation
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonAllerModeSimulation()
{
    // important de signaler le modele avant de faire le changement d'�tat, car
    // celui-ci utilise des informations du mod�le pour s'initialiser
    bool retour = FacadeModele::getInstance( )->changeControler( GAME_CONTROLLER_SIMULATION );
    return retour;
}

bool RepartiteurActions::actionBoutonAllerGameOfLife()
{
    // important de signaler le modele avant de faire le changement d'�tat, car
    // celui-ci utilise des informations du mod�le pour s'initialiser
    bool retour = FacadeModele::getInstance( )->changeControler( GAME_CONTROLLER_GAME_OF_LIFE );
    return retour;
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
    FacadeModele::getInstance()->togglePause();
    FacadeModele* wCrash = 0;
    wCrash->animer(0.3f);
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
//     Partie* wGame = FacadeModele::getInstance()->obtenirPartieCourante();
// 	if(wGame && (!wGame->isNetworkClientGame() || wGame->getGameStatus()==GAME_ENDED))
//     {
//         // Toggle
// 	    if(GestionnaireAnimations::obtenirInstance()->estJouerReplay())
// 	    {
//             // TODO:: refactor Animation
// // 		    vue::Vue* vueCourante = FacadeModele::getInstance()->obtenirVue();
// // 		    vue::Camera* camera = &vueCourante->obtenirCamera();
// // 		    vue::Camera ancienneCamera = GestionnaireAnimations::obtenirInstance()->obtenirAncienneCamera();
// // 
// // 		    AnimationFrame* frame[2];
// // 		    frame[0] = new AnimationFrame(0, camera->obtenirPosition(), camera->obtenirPointVise(), camera->obtenirDirectionHaut());
// // 		    frame[1] = new AnimationFrame(500, ancienneCamera.obtenirPosition(), ancienneCamera.obtenirPointVise(), ancienneCamera.obtenirDirectionHaut());
// // 
// // 		    Animation* animation = new Animation(BEZIER, true, true, true);
// // 		    for(int i=0; i<2; i++)
// // 			    animation->ajouterFrame(frame[i]);
// // 		    animation->ajouterObjet((ObjetAnimable*)&(vueCourante->obtenirCamera()));
// // 		    GestionnaireAnimations::obtenirInstance()->ajouterAnimation(animation);
// // 
// // 		    GestionnaireAnimations::obtenirInstance()->terminerReplay();
// 		    Partie* partie = FacadeModele::getInstance()->obtenirPartieCourante();
// 		    if(partie && !partie->partieTerminee())
// 			    partie->obtenirGameTime()->unPause();
// 	    }
// 	    else
// 	    {
// 		    vue::Vue* vueCourante = FacadeModele::getInstance()->obtenirVue();
// 		    vue::Camera* camera = &vueCourante->obtenirCamera();
// 		
// 		    vue::Camera ancienneCamera = GestionnaireAnimations::obtenirInstance()->obtenirAncienneCamera();
// 		    // Mettre quelque chose du genre pour effectuer un fondu au debut du replay
// 		    /*AnimationFrame* frame[2];
// 		    frame[0] = new AnimationFrame(0, camera->obtenirPosition(), camera->obtenirPointVise(), camera->obtenirDirectionHaut());
// 		    frame[1] = new AnimationFrame(500, ancienneCamera.obtenirPosition(), ancienneCamera.obtenirPointVise(), ancienneCamera.obtenirDirectionHaut());
// 
// 		    Animation* animation = new Animation(BEZIER, true, true, true);
// 		    for(int i=0; i<2; i++)
// 			    animation->ajouterFrame(frame[i]);
// 		    animation->ajouterObjet((ObjetAnimable*)&(vueCourante->obtenirCamera()));
// 		    GestionnaireAnimations::getInstance()->ajouterAnimation(animation);*/
// 		    FacadeModele::getInstance()->obtenirPartieCourante()->obtenirGameTime()->pause();
// 		    GestionnaireAnimations::obtenirInstance()->jouerReplay(FacadeModele::getInstance()->obtenirVue()->obtenirCamera());
// 	    }
//     }
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
    FacadeModele::getInstance()->obtenirVue()->obtenirProjection().obtenirCoordonneesCloture(xMinCourant, xMaxCourant, yMinCourant, yMaxCourant);

    vue::VuePerspectiveCiel* nouvelleVue = new vue::VuePerspectiveCiel(
        vue::Camera(Vecteur3(0, -50, 300), Vecteur3(0, 0, 0),
        Vecteur3(0, 1, 0)),
        0, 400, 0, 400,
        180, 50000, /*ZoomInMax*/10, /*ZoomOutMax*/15000, 1.25,
        -150, 150, -150, 150);
    nouvelleVue->redimensionnerFenetre(Vecteur2i(xMinCourant, yMinCourant), Vecteur2i(xMaxCourant, yMaxCourant));

    FacadeModele::getInstance()->resetCurrentZoom();
    FacadeModele::getInstance()->modifierVue(nouvelleVue);
    FacadeModele::getInstance()->obtenirVue()->centrerCamera(FacadeModele::getInstance()->getTableWidth());

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
    vue::Camera cameraCourante = FacadeModele::getInstance()->obtenirVue()->obtenirCamera();
    int xMinCourant, yMinCourant, xMaxCourant, yMaxCourant;
    FacadeModele::getInstance()->obtenirVue()->obtenirProjection().obtenirCoordonneesCloture(xMinCourant, xMaxCourant, yMinCourant, yMaxCourant);

    vue::VuePerspectiveOrbit* nouvelleVue = new vue::VuePerspectiveOrbit(
        cameraCourante,
        0, 400, 0, 400,
        180, 50000, /*ZoomInMax*/10, /*ZoomOutMax*/15000, 1.25,
        -150, 150, -150, 150);
    nouvelleVue->redimensionnerFenetre(Vecteur2i(xMinCourant, yMinCourant), Vecteur2i(xMaxCourant, yMaxCourant));

    FacadeModele::getInstance()->modifierVue(nouvelleVue);
    FacadeModele::getInstance()->obtenirVue()->centrerCamera(FacadeModele::getInstance()->getTableWidth());

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
    vue::Camera cameraCourante = FacadeModele::getInstance()->obtenirVue()->obtenirCamera();
    int xMinCourant, yMinCourant, xMaxCourant, yMaxCourant;
    FacadeModele::getInstance()->obtenirVue()->obtenirProjection().obtenirCoordonneesCloture(xMinCourant, xMaxCourant, yMinCourant, yMaxCourant);


    vue::VuePerspectiveLibre* nouvelleVue = new vue::VuePerspectiveLibre(
        cameraCourante,
        0, 300, 0, 400,
        180, 50000, /*ZoomInMax*/10, /*ZoomOutMax*/15000, 1.25,
        -150, 150, -150, 150);
    nouvelleVue->redimensionnerFenetre(Vecteur2i(xMinCourant, yMinCourant), Vecteur2i(xMaxCourant, yMaxCourant));

    FacadeModele::getInstance()->modifierVue(nouvelleVue);
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
    FacadeModele::getInstance()->obtenirVue()->obtenirProjection().obtenirCoordonneesCloture(xMinCourant, xMaxCourant, yMinCourant, yMaxCourant);

    vue::Vue* nouvelleVue = new vue::VuePerspectiveSplit(
        vue::Camera(Vecteur3(-200.0, 0.0, 75.0), Vecteur3(0, 0, 0),
        Vecteur3(0, 0, 1)),
        0, 400, 0, 400,
        180, 50000, /*ZoomInMax*/10, /*ZoomOutMax*/15000, 1.25,
        -150, 150, -150, 150);
    FacadeModele::getInstance()->obtenirVue()->centrerCamera(FacadeModele::getInstance()->getTableWidth());
    nouvelleVue->redimensionnerFenetre(Vecteur2i(xMinCourant, yMinCourant), Vecteur2i(xMaxCourant, yMaxCourant));


    FacadeModele::getInstance()->modifierVue(nouvelleVue);

    return true; 
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonUndo()
///
/// /*Description*/
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonUndo()
{
    return !!FacadeModele::getInstance()->getEditionField()->undoModification();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionBoutonRedo()
///
/// /*Description*/
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionBoutonRedo()
{
    return !!FacadeModele::getInstance()->getEditionField()->redoModification();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionReinitialiserRondelle()
///
///Fonction d'action pour reinitialiaser la rondelle
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionReinitialiserRondelle()
{
    Partie* wGame = FacadeModele::getInstance()->obtenirPartieCourante();
    if(wGame && wGame->isNetworkClientGame())
    {
        // On ne peut pas reset la puck directement, il faut demander au serveur de la reset et a la reception de la reponse, on la reset
        PaquetGameEvent* wPaquet = (PaquetGameEvent*) UsinePaquet::creerPaquet(GAME_EVENT);
        wPaquet->setEvent(GAME_EVENT_RESET_PUCK);
        wPaquet->setGameId(wGame->getUniqueGameId());
        RelayeurMessage::obtenirInstance()->relayerPaquetGame(wGame->getUniqueGameId(), wPaquet, TCP);
    }
    else
    {
        FacadeModele::getInstance()->reinitialiserRondelle();
    }
    return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool RepartiteurActions::actionPlayInEditor()
///
/// /*Description*/
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool RepartiteurActions::actionPlayInEditor()
{
    EventManager::modifierEtatSouris(ETAT_SOURIS_PIE_MODE);
    return true;
}






///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////