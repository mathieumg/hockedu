//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireAnimations.h
/// @author Mathieu Parent
/// @date 2012-03-18
/// @version 1.0 
///
/// @addtogroup utilitaire Utilitaire
/// @{
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Singleton.h"
#include "Animation.h"
#include "AnimationReprise.h"
#include "Vue\Camera.h"
#include "Vue\Vue.h"
#include "ReplayObserver.h"


///////////////////////////////////////////////////////////////////////////
/// @class GestionnaireAnimations
/// @brief Classe qui permet de gerer toutes les animations du programme
///
/// @author Mathieu Parent
/// @date 2012-03-18
///////////////////////////////////////////////////////////////////////////
class GestionnaireAnimations:public Singleton<GestionnaireAnimations>, public ReplaySubject
{
	SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(GestionnaireAnimations);
public:
	/// Constructeur par defaut
	GestionnaireAnimations();
	/// Destructeur
	~GestionnaireAnimations();

	/// Permet d'ajouter une animation
	void ajouterAnimation(Animation* animation);
	/// Permet de retirer une animation
	void retirerAnimation(Animation* animation);

	/// Permet d'animer les animations qu'il contient
	void animer(float temps);

	/// Delie un objet des animations
	void delierObjet(ObjetAnimable* objet);

	/// Sauvegarde des frames pour le replay
	void saveReplayFrame(IterationReplay* frame);
	/// Sauvegarde des frames pour le replay
	void saveReplaySound(int choix);

	/// Joueur le replay
	void jouerReplay(vue::Camera& camera);

	/// Accesseurs sur l'état du replay
	bool estJouerReplay() const;

	/// Mettre fin au replay
	void terminerReplay() 
    {
        mReplaying = false; 
        replay_->arreterAnimation();
        ReplaySubject::signalObservers();
    }

	/// Vider les frames du replay
	void viderBufferReplay();

	/// Accesseur sur la caméra
	vue::Camera obtenirAncienneCamera() const {return ancienneCamera_;}
	/// Effacer les animations de la caméra
	void viderAnimationCamera();
private:
	/// Pointeur vers l'instance unique de la classe.
	static GestionnaireAnimations* instance_;
	/// L'ancienne caméra
	vue::Camera ancienneCamera_;
	/// Liste des animations
	ListeAnimations animations_;

	/// Le replay
	AnimationReprise* replay_;

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////