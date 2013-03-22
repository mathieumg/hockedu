
#pragma once
#include "../Reseau/PaquetRunnable.h"


namespace PaquetRunnable
{
    // Delegates specifiques au client lourd

    int RunnableMailletClient(Paquet* pPaquet);

    int RunnableRondelleClient(Paquet* pPaquet);

    int RunnableGameConnectionClient(Paquet* pPaquet);

    int RunnableGameEventClient(Paquet* pPaquet);


};



