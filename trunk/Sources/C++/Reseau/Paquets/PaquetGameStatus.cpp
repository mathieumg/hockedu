#include "PaquetGameStatus.h"


PaquetGameStatus::PaquetGameStatus(  )
{
    mGame = new PartieServeurs;
}




PaquetGameStatus::~PaquetGameStatus()
{
    if(mGame)
    {
        delete mGame;
    }
}
