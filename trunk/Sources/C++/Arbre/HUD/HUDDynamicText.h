//////////////////////////////////////////////////////////////////////////////
/// @file HUDDynamicText.h
/// @author Michael Ferris
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "HUDElement.h"
#include <string>
#include <sstream>
#include <functional>


// the return value is the color of the text
typedef std::function<Vecteur4f( std::stringstream&)> GenerateDynamicText;

///////////////////////////////////////////////////////////////////////////
/// @class HUDDynamicText
/// @brief Classe qui repr�sente un �l�ment de texte dynamique pouvant �tre trac� dans le HUD.
///
///////////////////////////////////////////////////////////////////////////
class HUDDynamicText: public HUDElement
{
public : 
    /// Constructeurs par param�tres
    HUDDynamicText( GenerateDynamicText generator, bool isSmallText = true );

    /// Appliquer une couleur
    virtual void peindreElement();

    /// Generates the text
    std::string getText() const;
private :
    GenerateDynamicText mGenerator;
    /// La police de caract�re
    bool mIsSmallText;
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////



