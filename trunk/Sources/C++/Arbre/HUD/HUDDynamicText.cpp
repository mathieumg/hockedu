//////////////////////////////////////////////////////////////////////////////
/// @file HUDDynamicText.cpp
/// @author Michael Ferris
/// @date 2014-03-21
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "HUDDynamicText.h"
#include "FacadeModele.h"
#include "Partie.h"
#include "Tournoi.h"
#include <sstream>
#include "Utilitaire.h"
#include "PlayerAbstract.h"
#include <algorithm>
#include "GestionnaireHUD.h"
#include "phont.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn HUDDynamicText::HUDDynamicText(std::string* texte, Vecteur4 couleur)
///
/// Constructeur par paramètre.
///
/// @param[in] std::string* texte : le texte de l'élément.
/// @param[in] Vecteur4 couleur : la couleur du texte.
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
HUDDynamicText::HUDDynamicText( GenerateDynamicText generator, bool isSmallText )
    :mGenerator(generator),mIsSmallText(isSmallText)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void HUDDynamicText::peindreElement()
///
/// Application d'une couleur.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void HUDDynamicText::peindreElement()
{
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    std::stringstream text;
    Vecteur4f color( 1, 1, 1, 1 );
    if( mGenerator )
    {
        color = mGenerator( text );
    }
    glColor4f( color[0], color[1], color[2], color[3] );

    Phont* phont = GestionnaireHUD::obtenirInstance()->getTextRenderer(mIsSmallText);
    if(phont)
    {
        phont->print( obtenirX( ), 1 - obtenirY( ), true, text.str().c_str( ) );
    }
    glPopAttrib();
    glPopMatrix();
}

std::string HUDDynamicText::getText() const
{
    std::stringstream text;
    if( mGenerator )
    {
        mGenerator( text );
    }
    return text.str();
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


