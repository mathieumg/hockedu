
#pragma once
#include "..\Reseau\PaquetRunnable.h"


namespace PaquetRunnable
{
    // Delegates specifiques au serveur maitre

    int RunnableLoginInfoServerMaster(Paquet* pPaquet);

    int RunnableChatMessageServerMaster(Paquet* pPaquet);

    int RunnableUserStatusServerMaster(Paquet* pPaquet);

    int RunnableGameStatusServerMaster(Paquet* pPaquet);


};



