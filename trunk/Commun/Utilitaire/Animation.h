///////////////////////////////////////////////////////////////////////////////
/// @file Animation.h
/// @author Mathieu Parent
/// @date 2012-03-16
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <vector>
#include "Vecteur.h"

enum TypeAnimation {OBJET, CAMERAPOINT, CAMERALIBRE};
enum Interpolation {CONSTANT, LINEAIRE, BEZIER};
enum Attributs {ANIMPOSITION, ANIMANGLE, ANIMECHELLE};

class ObjetAnimable;
class AnimationFrame;
typedef std::vector<AnimationFrame*> ListeFrame;
typedef std::vector<ObjetAnimable*> ListeObjetsAnimable;


///////////////////////////////////////////////////////////////////////////
/// @class Animation
/// @brief Effectue les calculs pour l'animation de divers objets
///
/// @author Mathieu Parent
/// @date 2012-03-16
///////////////////////////////////////////////////////////////////////////
class Animation
{

public:
	/// Constructeurs
	Animation(int typeInterpolation,bool modParam1 = true, bool modParam2 = true, bool modParam3 = true, bool repeat = false);
	Animation(int mode, int typeInterpolation);
	/// Destructeur
	virtual ~Animation(void);

	/// Modifie le temps d'animation
	virtual void animer(float temps);

	/// Ajout d'élément
	virtual void ajouterFrame(AnimationFrame* frame);
	virtual void ajouterObjet(ObjetAnimable* objet);

	/// Affichage
	void afficher() const;

	/// Libération d'un objet
	virtual void delierObjet(ObjetAnimable* objet);

	/// Accesseur et modificateur de la frame courante
	virtual int obtenirFrameCourant() const { return indexFrameCourant_; }
	virtual void modifierFrameCourant(int val) { indexFrameCourant_ = val; }

	/// Accesseur et modificateur de l'interpolation
	int obtenirInterpolation() const { return interpolation_; }
	void modifierInterpolation(int val) { interpolation_ = val; }

	/// Accesseur sur le nombre d'objets
	int obtenirNbObjets() const;

	/// Calcul du bezier
	Vecteur3 calculerBezier(int attribut, float t, int indexPremier, int indexDernier);

	/// L'animation est-elle terminée?
	bool estTermine() const { return termine_; }

	/// Reset
	void resetTerminer();

	/// Arrêt de l'animation
	virtual void arreterAnimation() {termine_ = true;}

	/// Accesseur des objets animable 
	ObjetAnimable* obtenirObjets(unsigned int pos)  { if(pos >= objets_.size()) return 0; return objets_[pos]; }
protected:
	/// Types d'animation
	void animerConstant(AnimationFrame* courant);
	void animerLineaire(AnimationFrame* courant, AnimationFrame* suivant);
	void animerBezier();

	/// True si on doit repeter l'animation en boucle
	bool repeat_;

	/// True si l'animation est terminee
	bool termine_;
	
	/// Liste de tous les keyframes de l'animation
	ListeFrame frames_;

	/// Liste des objets affectes par l'animation
	ListeObjetsAnimable objets_;

	/// Keyframe courant
	int indexFrameCourant_;

	/// Temps courant
	float tempsCourant_;
	
	/// Type d'interpolation
	int interpolation_;

	/// Mode d'animation
	int mode_;

	/// Indique si on doit appliquer la modification aux objets
	bool modParam1_;
	bool modParam2_;
	bool modParam3_;
};

typedef std::vector<Animation*> ListeAnimations;

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////





