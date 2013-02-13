
#include "ControllerInterface.h"

PaquetRunnableFunc ControllerInterface::getRunnable( PaquetTypes pType ) const
{
    auto it = mPaquetRunnables.find(pType);
    if(it != mPaquetRunnables.end())
    {
        return it->second;
    }
    return NULL;
}
