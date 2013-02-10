//////////////////////////////////////////////////////////////////////////////
/// @file HUDElementComparatif.h
/// @author Gabriel Couvrette, Charles Étienne Lalonde
/// @date 2012-03-26
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "HUDElement.h"
#include <functional>

///////////////////////////////////////////////////////////////////////////////
/// @class HUDElementComparatif
/// @brief Classe qui représente un élément ayant une visibilité évaluée dynamiquement.
///
/// @author Gabriel Couvrette
/// @date 2012-03-09
//////////////////////////////////////////////////////////////////////////////
class HUDElementComparatif: public HUDElement
{
public:
	/// Constructeur par paramètre
	HUDElementComparatif(std::function<bool ()> fonctionPourComparer, Vecteur4f& couleurFond = Vecteur4f(0,0,0,0));
	/// Accesseur de la visibilité
	virtual bool estVisible();
protected:
	/// Coloriage de l'élément
	virtual void peindreElement();
private:
	/// Fonction appelee pour décider si l'élément doit être dessiné ou non
	std::function<bool ()> fonctionDeComparaison_;
	/// Valeur de comparaison
	bool valeurDeComparaison_;
	/// Couleur de fond
	Vecteur4f couleurFond_;
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////



