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


///////////////////////////////////////////////////////////////////////////
/// @class GestionnaireAnimations
/// @brief Classe qui permet de gerer toutes les animations du programme
///
/// @author Mathieu Parent
/// @date 2012-03-18
///////////////////////////////////////////////////////////////////////////
class GestionnaireAnimations:public Singleton<GestionnaireAnimations>
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
	void jouerReplay(vue::Camera camera);

	/// Accesseurs sur l'�tat du replay
	bool estJouerReplay() const;
	bool replayEstTermine();

	/// Mettre fin au replay
	void terminerReplay() {jouerReplay_ = false; replayTermine_ = true; replay_->arreterAnimation();}

	/// Vider les frames du replay
	void viderBufferReplay();

	/// Accesseur sur la cam�ra
	vue::Camera obtenirAncienneCamera() const {return ancienneCamera_;}
	/// Effacer les animations de la cam�ra
	void viderAnimationCamera();
private:
	/// Pointeur vers l'instance unique de la classe.
	static GestionnaireAnimations* instance_;
	/// L'ancienne cam�ra
	vue::Camera ancienneCamera_;
	/// Liste des animations
	ListeAnimations animations_;

	/// �tat du replay
	bool jouerReplay_;
	bool replayTermine_;

	/// Le replay
	AnimationReprise* replay_;

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////