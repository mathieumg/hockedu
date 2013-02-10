//////////////////////////////////////////////////////////////////////////////
/// @file ControllerInterface.h
/// @author Mathieu Parent
/// @date 2013-01-29
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once


///////////////////////////////////////////////////////////////////////////
/// @class ControllerInterface
/// @brief Interface a implementer pour un controlleur a associer au modele pour les callbacks
///
/// @author Mathieu Parent
/// @date 2013-01-29
///////////////////////////////////////////////////////////////////////////
class ControllerInterface
{
public:
    virtual void handleEvent(int pEventCode,  va_list pListeElems) = 0;

private:
    
};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

