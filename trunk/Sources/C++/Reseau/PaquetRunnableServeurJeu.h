
#pragma once
#include "../Reseau/PaquetRunnable.h"
#include "Paquets/PaquetBonus.h"
#include "Singleton.h"
#include <map>
#include <list>
#include "../Reseau/FacadePortability.h"

namespace PaquetRunnable
{
    // Delegates specifiques au serveur jeu
    
    int RunnableChatMessageServerGame(Paquet* pPaquet);

    int RunnableUserStatusServerGame(Paquet* pPaquet);

    int RunnableGameStatusServerGame(Paquet* pPaquet);

    int RunnableMailletServerGame(Paquet* pPaquet);

    int RunnableGameCreationServerGame(Paquet* pPaquet);

    int RunnableGameConnectionServerGame(Paquet* pPaquet);

    int RunnableGameEventServerGame(Paquet* pPaquet);    
};


class PaquetRunnableServeurJeuHelper : public Singleton<PaquetRunnableServeurJeuHelper>
{
    SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(PaquetRunnableServeurJeuHelper);
public:
    std::map<std::string, std::list<int>> mMapMapnameGameId;
    HANDLE_MUTEX mMutexMapMapnameGameId;
private:
    PaquetRunnableServeurJeuHelper();
};


