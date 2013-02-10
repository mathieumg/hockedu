//////////////////////////////////////////////////////////////////////////////
/// @file HUDElementComparatif.h
/// @author Gabriel Couvrette, Charles �tienne Lalonde
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
/// @brief Classe qui repr�sente un �l�ment ayant une visibilit� �valu�e dynamiquement.
///
/// @author Gabriel Couvrette
/// @date 2012-03-09
//////////////////////////////////////////////////////////////////////////////
class HUDElementComparatif: public HUDElement
{
public:
	/// Constructeur par param�tre
	HUDElementComparatif(std::function<bool ()> fonctionPourComparer, Vecteur4f& couleurFond = Vecteur4f(0,0,0,0));
	/// Accesseur de la visibilit�
	virtual bool estVisible();
protected:
	/// Coloriage de l'�l�ment
	virtual void peindreElement();
private:
	/// Fonction appelee pour d�cider si l'�l�ment doit �tre dessin� ou non
	std::function<bool ()> fonctionDeComparaison_;
	/// Valeur de comparaison
	bool valeurDeComparaison_;
	/// Couleur de fond
	Vecteur4f couleurFond_;
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////



