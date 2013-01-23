///////////////////////////////////////////////////////////////////////////////
/// @file AnimationFrame.h
/// @author Mathieu Parent
/// @date 2012-03-16
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Vecteur.h"

///////////////////////////////////////////////////////////////////////////
/// @class AnimationFrame
/// @brief Keyframe d'une animation
///
/// @author Mathieu Parent
/// @date 2012-03-16
///////////////////////////////////////////////////////////////////////////
class AnimationFrame
{
public:
	/// Constructeur par paramètres
	AnimationFrame(float temps, Vecteur3 param1, Vecteur3 param2 = Vecteur3(0, 0, 0), Vecteur3 param3 = Vecteur3(1, 1, 1));
	/// Destructeur
	~AnimationFrame(void);

	/// Méthode d'affichage
	void afficher() const;

	/// Accesseur et modificateur du temps
	float obtenirTemps() const { return temps_; }
	void modifierTemps(float val) { temps_ = val; }

	/// Accesseur et modificateur du premier paramètre
	Vecteur3 obtenirParam1() const { return param1_; }
	void modifierParam1_(Vecteur3 val) { param1_ = val; }

	/// Accesseur et modificateur du deuxième paramètre
	Vecteur3 obtenirParam2_() const { return param2_; }
	void modifierParam2_(Vecteur3 val) { param2_ = val; }

	/// Accesseur et modificateur du troisième paramètre
	Vecteur3 obtenirParam3_() const { return param3_; }
	void modifierParam3_(Vecteur3 val) { param3_ = val; }

	/// Surcharge de l'opérateur de comparaison <
	bool operator < (const AnimationFrame& a) const { return temps_ < a.temps_; }

protected:
	/// Temps a laquelle la frame doit etre effectuer
	float temps_;
	
	/// Attribut de position
	Vecteur3 param1_;
	
	/// Attribut de rotation en degres
	Vecteur3 param2_;
	
	/// Attribut de mise a lechelle
	Vecteur3 param3_;
	
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////





