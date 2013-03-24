
#pragma once
#include "../Reseau/PaquetRunnable.h"
#include "Paquets/PaquetBonus.h"


namespace PaquetRunnable
{
    // Delegates specifiques au serveur maitre

    int RunnableAuthentificationServeurJeuServerGame(Paquet* pPaquet);

    int RunnableChatMessageServerGame(Paquet* pPaquet);

    int RunnableUserStatusServerGame(Paquet* pPaquet);

    int RunnableGameStatusServerGame(Paquet* pPaquet);

    int RunnableMailletServerGame(Paquet* pPaquet);

    int RunnableGameCreationServerGame(Paquet* pPaquet);

    int RunnableGameConnectionServerGame(Paquet* pPaquet);

    int RunnableGameEventServerGame(Paquet* pPaquet);

    // Section Bonus
    int RunnableBonusMailletMuretServerGame(PaquetBonus* pPaquet);
    int RunnableBonusGoalerServerGame(PaquetBonus* pPaquet);


};



