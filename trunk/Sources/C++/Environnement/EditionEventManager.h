//////////////////////////////////////////////////////////////////////////////
/// @file EditionEventManager.h
/// @author Michael Ferris
/// @date 2013-03-24
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Enum_Declarations.h"

typedef void (*EditionEventReceived)(EditionEventCodes);

///////////////////////////////////////////////////////////////////////////
/// @class EditionEventManager
/// @brief Classe effectuant le pont entre l'interface et les evenements en mode edition
///
///
/// @author Michael Ferris
/// @date 2013-03-24
///////////////////////////////////////////////////////////////////////////
class EditionEventManager
{
public:
    static void TransmitEvent(EditionEventCodes pEvent){if(mEditionEventCallback)mEditionEventCallback(pEvent);}
    /// Accessors of mEditionEventCallback
    static inline void setEditionEventCallback(EditionEventReceived pVal) { mEditionEventCallback = pVal; }
private:
    static EditionEventReceived mEditionEventCallback;

    /// constructeur privé pour empeché l'instanciation
    EditionEventManager(){}
};



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

