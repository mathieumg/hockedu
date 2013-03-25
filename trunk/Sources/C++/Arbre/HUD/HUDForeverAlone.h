//////////////////////////////////////////////////////////////////////////////
/// @file HUDForeverAlone.h
/// @author Mathieu Parent
/// @date 2013-03-23
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "HUDElement.h"
#include "RazerGameTypeDef.h"
#include "Enum_Declarations.h"
#include "HUDSurfaceGL.h"

///////////////////////////////////////////////////////////////////////////////
/// @class HUDForeverAlone
/// @brief Classe qui représente une surface servant a dessiner Forever Alone dans une partie
///
/// @author Mathieu Parent
/// @date 2013-03-23
//////////////////////////////////////////////////////////////////////////////
class HUDForeverAlone : public HUDElement
{
public:
    /// Accessors of Modifiers
    inline void setModifiers(const ModifiersContainer* pVal) { mModifiers = pVal; }

    void initSurfaces();
    void clearSurfaces();
protected:
    /// Renders the element
    virtual void peindreElement();
private:
    /// Modifiers to render
    const ModifiersContainer* mModifiers;

    /// shared memory of element to render on the HUD. Simply adjust position and
    /// timer before rendering
    unsigned int mTexture[1];
    HUDSurfaceGL* mSurface;

};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////




