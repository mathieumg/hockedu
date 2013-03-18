//////////////////////////////////////////////////////////////////////////////
/// @file HUDBonus.h
/// @author Michael Ferris
/// @date 2013-03-18
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "HUDElement.h"
#include "RazerGameTypeDef.h"
#include "Enum_Declarations.h"

///////////////////////////////////////////////////////////////////////////////
/// @class HUDBonus
/// @brief Classe qui représente une surface servant a dessiner les bonus
///         des objets dans une partie
///
/// @author Michael Ferris
/// @date 2013-03-18
//////////////////////////////////////////////////////////////////////////////
class HUDBonus : public HUDElement
{
public:
    /// Accessors of Modifiers
    inline void setModifiers(const ModifiersContainer* pVal) { mModifiers = pVal; }

    static void initSurfaces();
    static void clearSurfaces();
protected:
    /// Renders the element
    virtual void peindreElement();
private:
    /// Modifiers to render
    const ModifiersContainer* mModifiers;

    /// shared memory of element to render on the HUD. Simply adjust position and
    /// timer before rendering
    static unsigned int mTextures[NB_BONUS_TYPE];
    static class HUDSurfaceGL* mSurface;
    static class HUDTexte* mTimerElement;
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////




