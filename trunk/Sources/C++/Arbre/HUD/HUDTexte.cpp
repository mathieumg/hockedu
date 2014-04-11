//////////////////////////////////////////////////////////////////////////////
/// @file HUDTexte.cpp
/// @author Gabriel Couvrette, Charles Étienne Lalonde
/// @date 2012-03-26
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "HUDTexte.h"
#include "GestionnaireHUD.h"
#include "phont.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn HUDTexte::HUDTexte(std::string* texte, Vecteur4 couleur)
///
/// Constructeur par paramètre.
///
/// @param[in] std::string* texte : le texte de l'élément.
/// @param[in] Vecteur4 couleur : la couleur du texte.
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
HUDTexte::HUDTexte(const std::string& texte, const Vecteur4f& couleur, bool isSmallText)
	:mText(texte), mColor(couleur),mIsSmallText(isSmallText)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void HUDTexte::peindreElement()
///
/// Application d'une couleur.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void HUDTexte::peindreElement()
{
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(mColor[0], mColor[1], mColor[2], mColor[3]);
	
    Phont* phont = GestionnaireHUD::obtenirInstance()->getTextRenderer(mIsSmallText);
    if(phont)
    {
        phont->print(obtenirX(),1-obtenirY(),true,mText.c_str());
    }
	glPopAttrib();
	glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


