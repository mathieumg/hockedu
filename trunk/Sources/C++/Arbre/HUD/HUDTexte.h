//////////////////////////////////////////////////////////////////////////////
/// @file HUDTexte.h
/// @author Gabriel Couvrette, Charles �tienne Lalonde
/// @date 2012-03-26
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "HUDElement.h"
#include <string>

///////////////////////////////////////////////////////////////////////////
/// @class HUDTexte
/// @brief Classe qui repr�sente un �l�ment pouvant �tre trac� dans le HUD.
///
/// @author Gabriel Couvrette, Charles �tienne Lalonde
/// @date 2012-03-09
///////////////////////////////////////////////////////////////////////////
class HUDTexte: public HUDElement
{
public : 
    /// Constructeurs par param�tres
    HUDTexte(const std::string& texte,const Vecteur4f& couleur, bool isSmallText = true);

    /// Appliquer une couleur
    virtual void peindreElement();

    /// Accessors of message_
    inline std::string getMessage() const { return mText; }
    inline void setMessage(const std::string& val) { mText = val; }
private :
    /// Le texte � afficher
    std::string mText;

    /// La police de caract�re
    bool mIsSmallText;
    /// La couleur du texte
    Vecteur4f mColor;
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////



