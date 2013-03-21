
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


// De base ne fait rien
void ControllerInterface::handleDisconnectDetection( SPSocket pSocket )
{

}


// Par defaut, ne fait rien
void ControllerInterface::getPlayersInGame( int pGameId, std::vector<const std::string>& pPlayerList )
{

}




Partie* ControllerInterface::getGame( int pGameId )
{
    return NULL;
}




