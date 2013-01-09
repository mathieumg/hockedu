///////////////////////////////////////////////////////////////////////////////
/// @file ObjetAnimable.h
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


///////////////////////////////////////////////////////////////////////////
/// @class ObjetAnimable
/// @brief Reprensente l'interface a implementer pour faire en sorte qu'un objet est animable
///
/// @author Mathieu Parent
/// @date 2012-03-16
///////////////////////////////////////////////////////////////////////////
class ObjetAnimable
{
public:
	/// Constructeur par défaut
	ObjetAnimable(void);
	/// Destructeur virtuel
	virtual ~ObjetAnimable(void);
	/// Méhode d'animation
	virtual void animerAnimation() = 0;

	/// Affichage
    virtual std::string obtenirNom() const = 0;

	/// Mise à jour des attributs
	void updateAttributs(Vecteur3 param1, Vecteur3 param2, Vecteur3 param3);

	/// Assignation des modificateurs
	void assignerModificateurs(bool modParam1, bool modParam2, bool modParam3);

	/// Accesseur et modificateur du premier paramètre
	Vecteur3 obtenirAnimationParam1() const { return animationParam1_; }
	void modifierAnimationParam1(Vecteur3 val) { animationParam1_ = val; }

	/// Accesseur et modificateur du deuxième paramètre
	Vecteur3 obtenirAnimationParam2() const { return animationParam2_; }
	void modifierAnimationParam2(Vecteur3 val) { animationParam2_ = val; }

	/// Accesseur et modificateur du troisième paramètre
	Vecteur3 otenirAnimationParam3() const { return animationParam3_; }
	void modifierAnimationParam3(Vecteur3 val) { animationParam3_ = val; }

protected:
	/// Attributs du premier paramètre
	bool modParam1_;
	Vecteur3 animationParam1_;
	
	/// Attributs du deuxième paramètre
	bool modParam2_;
	Vecteur3 animationParam2_;
	
	/// Attributs du troisième paramètre
	bool modParam3_;
	Vecteur3 animationParam3_;
	

};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////





