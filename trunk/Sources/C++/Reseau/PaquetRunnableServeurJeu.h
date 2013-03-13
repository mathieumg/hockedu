
#pragma once
#include "../Reseau/PaquetRunnable.h"


namespace PaquetRunnable
{
    // Delegates specifiques au serveur maitre

    int RunnableAuthentificationServeurJeuServerGame(Paquet* pPaquet);

    int RunnableChatMessageServerGame(Paquet* pPaquet);

    int RunnableUserStatusServerGame(Paquet* pPaquet);

    int RunnableGameStatusServerGame(Paquet* pPaquet);

    int RunnableMailletServerGame(Paquet* pPaquet);


};



