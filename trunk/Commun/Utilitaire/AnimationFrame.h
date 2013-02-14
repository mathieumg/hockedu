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
	/// Constructeur par param�tres
	AnimationFrame(float temps, const Vecteur3& pos, const Vecteur3& angle = Vecteur3(0, 0, 0), const Vecteur3& scale = Vecteur3(1, 1, 1));
	/// Destructeur
	~AnimationFrame(void);

	/// M�thode d'affichage
	void afficher() const;

	/// Accesseur et modificateur du temps
	float obtenirTemps() const { return temps_; }
	void modifierTemps(float val) { temps_ = val; }

	/// Accesseur et modificateur du premier param�tre
	inline const Vecteur3& getPosition() const { return mParams.mPosition; }
	inline void setPosition(const Vecteur3& val) { mParams.mPosition = val; }

	/// Accesseur et modificateur du deuxi�me param�tre
	inline const Vecteur3& getAngle() const { return mParams.mAngle; }
	inline void setAngle(const Vecteur3& val) { mParams.mAngle = val; }

	/// Accesseur et modificateur du troisi�me param�tre
	inline const Vecteur3& getScale() const { return mParams.mScale; }
	inline void setScale(const Vecteur3& val) { mParams.mScale = val; }

	/// Surcharge de l'op�rateur de comparaison <
	bool operator < (const AnimationFrame& a) const { return temps_ < a.temps_; }

protected:
	/// Temps a laquelle la frame doit etre effectuer
	float temps_;
    ObjectAnimationParameters mParams;
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////





