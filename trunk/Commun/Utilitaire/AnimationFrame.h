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
#include "ObjetAnimable.h"

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
	AnimationFrame(float temps, const Vecteur3& pos, const Vecteur3& angle = Vecteur3(0, 0, 0), const Vecteur3& scale = Vecteur3(1, 1, 1));
	/// Destructeur
	~AnimationFrame(void);

	/// Méthode d'affichage
	void afficher() const;

	/// Accesseur et modificateur du temps
	float obtenirTemps() const { return temps_; }
	void modifierTemps(float val) { temps_ = val; }

	/// Accesseur et modificateur du premier paramètre
	inline const Vecteur3& getPosition() const { return mParams.mPosition; }
	inline void setPosition(const Vecteur3& val) { mParams.mPosition = val; }

	/// Accesseur et modificateur du deuxième paramètre
	inline const Vecteur3& getAngle() const { return mParams.mAngle; }
	inline void setAngle(const Vecteur3& val) { mParams.mAngle = val; }

	/// Accesseur et modificateur du troisième paramètre
	inline const Vecteur3& getScale() const { return mParams.mScale; }
	inline void setScale(const Vecteur3& val) { mParams.mScale = val; }

	/// Surcharge de l'opérateur de comparaison <
	bool operator < (const AnimationFrame& a) const { return temps_ < a.temps_; }

protected:
	/// Temps a laquelle la frame doit etre effectuer
	float temps_;
    ObjectAnimationParameters mParams;
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////





