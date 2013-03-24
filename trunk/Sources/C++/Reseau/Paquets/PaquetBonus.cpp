

#include "PaquetBonus.h"


PaquetBonus::PaquetBonus(  )
{
    mInfos = NULL;
}




PaquetBonus::~PaquetBonus()
{
    if(mInfos)
    {
	    delete mInfos;
    }
}


