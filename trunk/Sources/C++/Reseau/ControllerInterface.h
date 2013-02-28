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
#ifdef WINDOWS
#include <hash_map>
#elif defined(LINUX)
#include <ext/hash_map>
namespace std {using namespace __gnu_cxx; }
#endif
#include "Socket.h"
#include "../../C#/UIHeavyClient/UIHeavyClient/Enum_Declarations.cs"

typedef int PaquetTypes;
typedef int (*PaquetRunnableFunc) (class Paquet*);

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
    PaquetRunnableFunc getRunnable(PaquetTypes pType) const;


    virtual void handleEvent(EventCodes pEventCode,  va_list pListeElems) = 0;
    virtual void handleDisconnectDetection(SPSocket pSocket);

protected:
    std::hash_map<PaquetTypes, PaquetRunnableFunc> mPaquetRunnables;
};




///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

