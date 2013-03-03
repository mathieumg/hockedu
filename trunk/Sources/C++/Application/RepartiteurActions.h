//////////////////////////////////////////////////////////////////////////////
/// @file RepartiteurActions.h
/// @author Michael Ferris
/// @date 2012-03-09
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <map>
#include <string>
#include "Singleton.h"
#include "Enum_Declarations.cs"
enum ModesCamera {CAMERA_FIXE, CAMERA_ORBIT, CAMERA_LIBRE};

/* Pour toute modification, veuillez mettre a jour la map dans
Java_ca_polymtl_inf2990_GestionnaireEvenements_actionPerformed()   pour la compatibilité java
et dans 
.cs
*/


///////////////////////////////////////////////////////////////////////////
/// @class FacadeModele
/// @brief Classe qui permet de repartir les actions recu par le java selon les besoins
///
/// @author Michael Ferris
/// @date 2012-03-09
///////////////////////////////////////////////////////////////////////////
class RepartiteurActions:public Singleton<RepartiteurActions>
{
	SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(RepartiteurActions);
public:
	/// Constructeur par defaut
	RepartiteurActions();
	/// Destructeur
	~RepartiteurActions();

	/// Fonction qui appelle les fonctions de la banque d'actions
	bool appelerMethodeAction(ActionType action);
	///Fonction d'action pour changer l'état de la souris sur Déplacer Fenêtre
	bool actionBoutonCamera();
	/// Fonction d'action pour changer l'état de la souris sur tourner la camera Fenêtre
	bool actionBoutonOrbit();
	///Fonction d'Action pour changer l'état de la souris sur Zoom Élastique
	bool actionBoutonZoomElastique();
	///Fonction d'Action pour changer l'état de la souris sur Zoom Proportionnel
	bool actionBoutonZoomProportionnel();
	///Fonction d'action pour gérer la sélection
	bool actionBoutonSelection();
	///Fonction d'action pour gérer les déplacements
	bool actionBoutonTransformationDeplacement();
	///Fonction d'Action pour changer l'état de la souris sur Trasformation Rotation
	bool actionBoutonTransformationRotation();
	///Fonction d'Action pour changer l'état de la souris sur Trasformation Échelle
	bool actionBoutonTransformationEchelle();
	///Fonction d'action pour supprimer la sélection actuelle.
	bool actionBoutonSupprimer();
	///Fonction d'action de création d'un nouveau fichier d'edition.
	bool actionBoutonEditionNouveau();
	///Fonction d'action pour reinitialisaer une partie
	bool actionReinitialiserPartie();
	///Fonction d'action pour inserer un portail dans l'espace de jeu.
	bool actionBoutonInsererPortail();
	///Fonction d'action pour inserer un muret dans l'espace de jeu.
	bool actionBoutonInsererMuret();
	///Fonction d'action pour inserer un maillet dans l'espace de jeu.
	bool actionBoutonInsererMaillet();
	///Fonction pour inserer les rondelles.
	bool actionBoutonInsererRondelle();
	///Fonction d'action pour inserer un accelerateur.
	bool actionBoutonInsererAccelerateur();
	///Fonction d'action pour dupliquer
	bool actionBoutonDupliquer();
	///Fonction pour aller en mode jeu
	bool actionBoutonAllerModeJeu();
	///Fonction pour aller en mode édition
	bool actionBoutonAllerModeEdition();
	///Fonction pour aller en mode tournoi
	bool actionBoutonAllerModeTournoi();
	///Fonction pour aller au menu principal
    bool actionBoutonAllerMenuPrincipal();
    ///Fonction pour aller au mode simulation
    bool actionBoutonAllerModeSimulation();
	///Fonction pour mettre le jeu en pause
	bool actionTogglePauseJeu();
	///Fonction pour jouer un replay
	bool actionReplay();
	///Fonction pour changer le mode de camera
	bool actionChangerModeCameraFixe();
	///Fonction pour changer le mode de camera
	bool actionChangerModeCameraOrbite();
	///Fonction pour changer le mode de camera
	bool actionChangerModeCameraLibre();
	///Fonction pour changer le mode de camera
	bool actionChangerModeCameraSplit();
private:
	/// typedef pour une adresse de fonction de RepartiteurActions
	typedef bool (RepartiteurActions::*ObtenirFonction)();
	/// Typedef pour la map qui va appeler les fonctions à partir de la commande d'un événement.
	typedef std::map<ActionType, ObtenirFonction> BanqueFonctions;
	/// Variable pour la traduction clé -> fonction pour les actions d'événement
	BanqueFonctions banqueActions_;

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




