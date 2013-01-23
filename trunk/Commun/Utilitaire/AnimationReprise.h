///////////////////////////////////////////////////////////////////////////////
/// @file AnimationReprise.h
/// @author Mathieu Parent
/// @date 2012-03-16
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Animation.h"
#include <list>
#include <utility>



typedef std::pair<AnimationFrame*, ObjetAnimable*> AnimationRepriseFrame;
//typedef std::pair<float, std::vector<AnimationRepriseFrame*>> IterationReplay;


struct IterationReplay
{
	IterationReplay(float temps, std::vector<AnimationRepriseFrame*> liste)
	{
		temps_ = temps;
		listeAnimationRepriseFrame_ = liste;
		choixSon_ = -1;
	}

	float obtenirTemps()
	{
		return temps_;
	}

	std::vector<AnimationRepriseFrame*>& obtenirListe()
	{
		return listeAnimationRepriseFrame_;
	}

	int obtenirChoixSon()
	{
		return choixSon_;
	}

	void modifierTemps(float temps)
	{
		temps_ = temps;
	}

	void modifierChoixSon(int choix)
	{
		choixSon_ = choix;
	}

	float temps_;
	std::vector<AnimationRepriseFrame*> listeAnimationRepriseFrame_;
	int choixSon_; // -1 si vide

};


typedef std::list<IterationReplay*> ListeFrameReprise;

///////////////////////////////////////////////////////////////////////////
/// @class AnimationReprise
/// @brief Effectue les calculs pour l'AnimationReprise de divers objets
///
/// @author Mathieu Parent
/// @date 2012-03-16
///////////////////////////////////////////////////////////////////////////
class AnimationReprise : public Animation
{
public:
	/// Constructeur par paramètres
	AnimationReprise(int typeInterpolation,bool modParam1 = true, bool modParam2 = false, bool modParam3 = false, bool repeat = false);
	/// Destructeur
	~AnimationReprise(void);

	/// Ajouter les éléments du replay
	virtual void ajouterFrame(IterationReplay* frame);
	virtual void ajouterSon(int choix);
	virtual void ajouterObjet(ObjetAnimable* objet);

	/// Libération d'un objet
	virtual void delierObjet(ObjetAnimable* objet);

	/// Arrêt de l'animation
	virtual void arreterAnimation();

	/// Accesseur sur la durée
	float obtenirDureeTotale();

	/// Animer
	virtual void animer( float temps );
	/// Vider les frames
	void viderListe();

private:
	/// Les frames de l'animation
	ListeFrameReprise frames_;
	ListeFrameReprise::iterator frameCourant_;
};


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////





