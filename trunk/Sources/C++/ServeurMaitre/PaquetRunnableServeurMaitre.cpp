

#include "PaquetRunnableServeurMaitre.h"
#include "../ServeurMaitre/FacadeServeurMaitre.h"
#include "../Reseau/Paquets/PaquetLoginInfo.h"
#include "../Reseau/GestionnaireReseau.h"
#include "../Reseau/ObjetsGlobaux/JoueurServeurs.h"
#include "../Reseau/RelayeurMessage.h"
#include "../Reseau/Paquets/PaquetUserStatus.h"
#include "../Reseau/Paquets/PaquetChatMessage.h"

#ifdef LINUX
#define _LARGE_TIME_API
#endif

#include <time.h>
#include <sstream>
#include <iomanip>


/// ***** PAR CONVENTION, METTRE Master A LA FIN DU NOM DES DELEGATES


int PaquetRunnable::RunnableLoginInfoServerMaster( Paquet* pPaquet )
{
    PaquetLoginInfo* wPaquet = (PaquetLoginInfo*) pPaquet;

    // Code pour l'authentification des users du cote du serveur maitre


    // On envoie un event au gestionnaire reseau
    GestionnaireReseau::obtenirInstance()->transmitEvent(SERVER_USER_CONNECTING, wPaquet->getUsername().c_str());

    // On sauvearde le joueur
    JoueurServeurs* wJoueur = new JoueurServeurs(wPaquet->getUsername());
    FacadeServeurMaitre::obtenirInstance()->saveJoueurConnecting(wJoueur);

    // On traite la demande avec la BD



    return 0;
}





int PaquetRunnable::RunnableChatMessageServerMaster( Paquet* pPaquet )
{
    PaquetChatMessage* wPaquet = (PaquetChatMessage*) pPaquet;


    std::ostringstream wTimeOutput;
    time_t wT = time(0);
    struct tm* wTime;
#ifdef WINDOWS
    if(_localtime64_s(wTime, &wT))
#elif defined(LINUX)
    time(&wT);
    wTime = localtime(&wT);
    if(wTime == NULL)
#endif
    {
        std::cout << "[00:00:00]";
    }
    else
    {
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



int PaquetRunnable::RunnableUserStatusServerMaster( Paquet* pPaquet )
{
    PaquetUserStatus* wPaquet = (PaquetUserStatus*) pPaquet;


    return 0;
}
