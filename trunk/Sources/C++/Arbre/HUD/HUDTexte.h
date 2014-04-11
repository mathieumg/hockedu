//////////////////////////////////////////////////////////////////////////////
/// @file HUDTexte.h
/// @author Gabriel Couvrette, Charles Étienne Lalonde
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
/// @brief Classe qui représente un élément pouvant être tracé dans le HUD.
///
/// @author Gabriel Couvrette, Charles Étienne Lalonde
/// @date 2012-03-09
///////////////////////////////////////////////////////////////////////////
class HUDTexte: public HUDElement
{
public : 
    /// Constructeurs par paramètres
    HUDTexte(const std::string& texte,const Vecteur4f& couleur, bool isSmallText = true);

    /// Appliquer une couleur
    virtual void peindreElement();

    /// Accessors of message_
    inline std::string getMessage() const { return mText; }
    inline void setMessage(const std::string& val) { mText = val; }
private :
    /// Le texte à afficher
    std::string mText;

    /// La police de caractère
    bool mIsSmallText;
    /// La couleur du texte
    Vecteur4f mColor;
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////



