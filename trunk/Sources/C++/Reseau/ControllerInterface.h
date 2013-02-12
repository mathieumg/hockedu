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
#include <hash_map>

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
    virtual void handleEvent(int pEventCode,  va_list pListeElems) = 0;
    PaquetRunnableFunc getRunnable(PaquetTypes pType) const;

protected:
    std::hash_map<PaquetTypes, PaquetRunnableFunc> mPaquetRunnables;
};



PaquetRunnableFunc ControllerInterface::getRunnable( PaquetTypes pType ) const
{
    auto it = mPaquetRunnables.find(pType);
    if(it != mPaquetRunnables.end())
    {
        return it->second;
    }
    return NULL;
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

