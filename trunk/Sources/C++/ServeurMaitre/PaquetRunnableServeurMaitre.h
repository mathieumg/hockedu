
#pragma once
#include "../Reseau/PaquetRunnable.h"


namespace PaquetRunnable
{
    // Delegates specifiques au serveur maitre

    int RunnableLoginInfoMasterServer(Paquet* pPaquet);

    int RunnableChatMessageMasterServer(Paquet* pPaquet);

    int RunnableUserStatusMasterServer(Paquet* pPaquet);

    int RunnableGameStatusMasterServer(Paquet* pPaquet);

    int RunnableGameCreationMasterServer( Paquet* pPaquet );

    int RunnableGameConnectionMasterServer( Paquet* pPaquet );

};



