//////////////////////////////////////////////////////////////////////////////
/// @file HUDElementComparatif.cpp
/// @author Gabriel Couvrette, Charles Étienne Lalonde
/// @date 2012-03-26
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "HUDElementComparatif.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn HUDElementComparatif::HUDElementComparatif(std::function<bool ()> fonctionPourComparer, Vecteur4f& couleurFond)
///
/// Constructeur par paramètres.
///
/// @param[in] std::function<bool ()> fonctionPourComparer.
/// @param[in] Vecteur4& couleur : la couleur de fond.
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
HUDElementComparatif::HUDElementComparatif(std::function<bool ()> fonctionPourComparer, Vecteur4f& couleurFond)
	:fonctionDeComparaison_(fonctionPourComparer), couleurFond_(couleurFond)
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool HUDElementComparatif::estVisible()
///
/// Accesseur sur la visibilité.
///
/// @return : la visibilité.
///
////////////////////////////////////////////////////////////////////////
bool HUDElementComparatif::estVisible()
{
	return fonctionDeComparaison_();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn HUDElementComparatif::peindreElement()
///
/// Peinture un élément.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void HUDElementComparatif::peindreElement()
{
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(couleurFond_[0], couleurFond_[1], couleurFond_[2], couleurFond_[3]);
	glBegin(GL_QUADS);
		glVertex2f(obtenirX(), obtenirY());
		glVertex2f(obtenirX(), obtenirY()+obtenirHauteur());
		glVertex2f(obtenirX()+obtenirLargeur(), obtenirY()+obtenirHauteur());
		glVertex2f(obtenirX()+obtenirLargeur(), obtenirY());
	glEnd();
	glPopAttrib();
	glPopMatrix();
}



/*
bool (*fonctionDeComparaison_)(float);//fonction appelee pour decider si lelement doit etre dessiner ou non.
float valeurDeComparaison_;
*/

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////



