
#pragma once
#include "../Reseau/PaquetRunnable.h"
#include "Paquets/PaquetBonus.h"


namespace PaquetRunnable
{
    // Delegates specifiques au client lourd

    int RunnableMailletClient(Paquet* pPaquet);

    int RunnableRondelleClient(Paquet* pPaquet);

    int RunnableGameConnectionClient(Paquet* pPaquet);

    int RunnableGameEventClient(Paquet* pPaquet);

    // Section bonus
    int RunnableBonusMailletMuretClient(PaquetBonus* pPaquet);
    int RunnableBonusGoalerClient(PaquetBonus* pPaquet);



};



