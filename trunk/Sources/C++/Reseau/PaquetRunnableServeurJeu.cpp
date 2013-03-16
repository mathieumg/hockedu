

#include "PaquetRunnableServeurJeu.h"

#ifdef LINUX
#define _LARGE_TIME_API
#endif

#include <time.h>
#include <sstream>
#include <iomanip>
#include <stdexcept>

#include "Paquets\PaquetAuthentificationServeurJeu.h"
#include "Paquets\PaquetChatMessage.h"
#include "RelayeurMessage.h"
#include "Paquets\PaquetUserStatus.h"
#include "Paquets\PaquetGameStatus.h"
#include "Paquets\PaquetMaillet.h"
#include "Paquets\PaquetGameCreation.h"
#include "GameManager.h"


/// ***** PAR CONVENTION, METTRE Game A LA FIN DU NOM DES DELEGATES


int PaquetRunnable::RunnableAuthentificationServeurJeuServerGame( Paquet* pPaquet )
{
    PaquetChatMessage* wPaquet = (PaquetChatMessage*) pPaquet;

    std::ostringstream wTimeOutput;
    time_t wT = time(0);
    struct tm* wTime = NULL;
#ifdef WINDOWS
    struct tm wTimeWin;
    if(_localtime64_s(&wTimeWin, &wT))
    {
#elif defined(LINUX)
    time(&wT);
    wTime = localtime(&wT);
    if(wTime == NULL)
    {
#endif

        // If time == NULL
        std::cout << "[00:00:00]";
    }
    else
    {
#ifdef WINDOWS
        wTime = &wTimeWin;
#endif
        wTimeOutput << std::setfill('0') << "["
            << std::setw(2) << wTime->tm_hour
            << std::setw(1) << ":"
            << std::setw(2) << wTime->tm_min
            << std::setw(1) << ":"
            << std::setw(2) << wTime->tm_sec
            << std::setw(1) << "]";
    }


    wTimeOutput << "[" << wPaquet->getOrigin() << "]: " << wPaquet->getMessage() << std::endl;
    std::cout << wTimeOutput.str();

    // On veut relayer le message a une personne en particulier ou a tout le monde
    // On ne call donc pas delete dessus tout suite
    if(wPaquet->IsTargetGroup())
    {
        // On l'envoie a la personne dans groupName seulement
        RelayeurMessage::obtenirInstance()->relayerPaquet(wPaquet->getGroupName(), wPaquet, TCP);
    }
    else
    {
        // On envoie a tout le monde
        RelayeurMessage::obtenirInstance()->relayerPaquetGlobalement(wPaquet, NULL, TCP);
    }

    return 0;
}





int PaquetRunnable::RunnableChatMessageServerGame( Paquet* pPaquet )
{
    PaquetChatMessage* wPaquet = (PaquetChatMessage*) pPaquet;


    std::ostringstream wTimeOutput;
    time_t wT = time(0);
    struct tm* wTime = NULL;
#ifdef WINDOWS
    struct tm wTimeWin;
    if(_localtime64_s(&wTimeWin, &wT))
    {
#elif defined(LINUX)
    time(&wT);
    wTime = localtime(&wT);
    if(wTime == NULL)
    {
#endif

        // If time == NULL
        std::cout << "[00:00:00]";
    }
    else
    {
#ifdef WINDOWS
        wTime = &wTimeWin;
#endif
        wTimeOutput << std::setfill('0') << "["
            << std::setw(2) << wTime->tm_hour
            << std::setw(1) << ":"
            << std::setw(2) << wTime->tm_min
            << std::setw(1) << ":"
            << std::setw(2) << wTime->tm_sec
            << std::setw(1) << "]";
    }


    wTimeOutput << "[" << wPaquet->getOrigin() << "]: " << wPaquet->getMessage() << std::endl;
    std::cout << wTimeOutput.str();

    // On veut relayer le message a une personne en particulier ou a tout le monde
    // On ne call donc pas delete dessus tout suite
    if(wPaquet->IsTargetGroup())
    {
        // On envoie a tout le monde
        RelayeurMessage::obtenirInstance()->relayerPaquetGlobalement(wPaquet, NULL, TCP);
    }
    else
    {
        // On l'envoie a la personne dans groupName seulement
        RelayeurMessage::obtenirInstance()->relayerPaquet(wPaquet->getGroupName(), wPaquet, TCP);
    }

    return 0;
}



int PaquetRunnable::RunnableUserStatusServerGame( Paquet* pPaquet )
{
    PaquetUserStatus* wPaquet = (PaquetUserStatus*) pPaquet;
    //throw std::runtime_error("Not yet implemented");

    return 0;
}

int PaquetRunnable::RunnableGameStatusServerGame( Paquet* pPaquet )
{
    PaquetGameStatus* wPaquet = (PaquetGameStatus*) pPaquet;




    return 0;
}


int PaquetRunnable::RunnableMailletServerGame( Paquet* pPaquet )
{
    PaquetMaillet* wPaquet = (PaquetMaillet*) pPaquet;

    // Relayer le message aux clients connectes qui font parti de cette partie
    // Plus tard, on ne devrait pas relayer directement, mais valider que les positions fonctionnent
    RelayeurMessage::obtenirInstance()->relayerPaquetGame(wPaquet->getGameId(), pPaquet);


    




    return 0;
}



int PaquetRunnable::RunnableGameCreationServerGame( Paquet* pPaquet )
{
    PaquetGameCreation* wPaquet = (PaquetGameCreation*) pPaquet;
    
    // Demande de creation d'une nouvelle partie.

    // On regarde d'abord si une partie avec ce nom existe deja
    if(!GameManager::obtenirInstance()->getGame(wPaquet->getGameName()))
    {
        // On peut creer la partie
        int wGameId = GameManager::obtenirInstance()->addNewGame(SPJoueurAbstrait(0), SPJoueurAbstrait(0), true);
        GameManager::obtenirInstance()->getGame(wGameId)->setName(wPaquet->getGameName());

        // On peut meme utiliser le meme paquet pour renvoyer le message de confirmation
        wPaquet->setGameId(wGameId);
        

        // En envoie le message de confirmation
        GestionnaireReseau::obtenirInstance()->envoyerPaquet(wPaquet->getUsername(), wPaquet, TCP);

    }
    else
    {
        // Une partie avec ce nom existe deja, on renvoie un message pour dire que la creation a echouee
        // Si creation a ecouee, id = -1

        // On peut meme utiliser le meme paquet pour renvoyer le message de confirmation
        wPaquet->setGameId(-1);

        // En envoie le message de confirmation
        GestionnaireReseau::obtenirInstance()->envoyerPaquet(wPaquet->getUsername(), wPaquet, TCP);

    }

    return 0;
}
