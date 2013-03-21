

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
#include "Paquets\PaquetGameConnection.h"
#include "RazerGameTypeDef.h"
#include "JoueurNetworkServeur.h"
#include "Runnable.h"
#include "NoeudMaillet.h"
#include "Paquets\PaquetGameRegistration.h"
#include "GestionnaireReseau.h"
#include "ControllerServeurJeu.h"
#include "JoueurVirtuel.h"

/// ***** PAR CONVENTION, METTRE Game A LA FIN DU NOM DES DELEGATES


int PaquetRunnable::RunnableAuthentificationServeurJeuServerGame( Paquet* pPaquet )
{
    PaquetChatMessage* wPaquet = (PaquetChatMessage*) pPaquet;

    std::ostringstream wTimeOutput;
    time_t wT = time(0);
    struct tm wTime;
    if(_localtime64_s(&wTime, &wT))
    {
        // If time == NULL
        std::cout << "[00:00:00]";
    }
    else
    {
        wTimeOutput << std::setfill('0') << "["
            << std::setw(2) << wTime.tm_hour
            << std::setw(1) << ":"
            << std::setw(2) << wTime.tm_min
            << std::setw(1) << ":"
            << std::setw(2) << wTime.tm_sec
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
    struct tm wTime;
    if(_localtime64_s(&wTime, &wT))
    {
        // If time == NULL
        std::cout << "[00:00:00]";
    }
    else
    {
        wTimeOutput << std::setfill('0') << "["
            << std::setw(2) << wTime.tm_hour
            << std::setw(1) << ":"
            << std::setw(2) << wTime.tm_min
            << std::setw(1) << ":"
            << std::setw(2) << wTime.tm_sec
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


    Partie* wGame = GameManager::obtenirInstance()->getGame(wPaquet->getGameId());

    if(wGame)
    {
        Vecteur3 wPos = wPaquet->getPosition();
        NoeudMaillet* maillet;
        if(wPaquet->getEstAGauche())
        {
            maillet = wGame->obtenirJoueurGauche()->getControlingMallet();
        }
        else
        {
            maillet = wGame->obtenirJoueurDroit()->getControlingMallet();
        }

        // On ne met pas a jour si c'est nous
        if(maillet)
        {
            JoueurAbstrait* wJoueur = maillet->obtenirJoueur();
            if(wJoueur && wJoueur->obtenirType() == JOUEUR_NETWORK_SERVEUR)
            {
                Runnable* r = new Runnable([maillet,wPos](Runnable*){

                    // Mettre la position du maillet
                    maillet->setTargetDestination(wPos);

                });
                //maillet->attach(r);
                RazerGameUtilities::RunOnUpdateThread(r,true);
            }
        }
        
        

        
    }

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

        if(!ControllerServeurJeu::isLocalServer())
        {
            PaquetGameRegistration* wPaquetRegistration = (PaquetGameRegistration*)GestionnaireReseau::obtenirInstance()->creerPaquet(GAME_REGISTRATION);
            wPaquetRegistration->setServerId(((ControllerServeurJeu*)GestionnaireReseau::obtenirInstance()->getController())->getServerId());
            wPaquetRegistration->setGameId(wGameId);
            wPaquetRegistration->setGameName(wPaquet->getGameName());
            wPaquetRegistration->setMapName(wPaquet->getMapName());
            wPaquetRegistration->setUsername(wPaquet->getUsername());

            GestionnaireReseau::obtenirInstance()->envoyerPaquet("MasterServer", wPaquetRegistration, TCP);
        }
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



GameConnectionState connectPlayer(const std::string& pPlayerName, Partie* pGame)
{
    if(pGame)
    {
        if(!pGame->obtenirJoueurGauche())
        {
            // Pas de joueur gauche. On assigne le joueur la
            pGame->assignerJoueur(SPJoueurNetworkServeur(new JoueurNetworkServeur(pPlayerName)));
            return GAME_CONNECTION_ACCEPTED_LEFT;
        }
        else if(pGame->obtenirNomJoueurGauche() == pPlayerName)
        {
            return GAME_CONNECTION_ALREADY_CONNECTED;
        }
        else if(!pGame->obtenirJoueurDroit())
        {
            // Pas de joueur droit. On assigne le joueur la
            pGame->assignerJoueur(SPJoueurNetworkServeur(new JoueurNetworkServeur(pPlayerName)));
            return GAME_CONNECTION_ACCEPTED_RIGHT;
        }
        else if(pGame->obtenirNomJoueurDroit() == pPlayerName)
        {
            return GAME_CONNECTION_ALREADY_CONNECTED;
        }
        else
        {
            return GAME_CONNECTION_GAME_FULL;
        }

    }
    else
    {
        return GAME_CONNECTION_REJECTED;
    }
}



int PaquetRunnable::RunnableGameConnectionServerGame( Paquet* pPaquet )
{
    PaquetGameConnection* wPaquet = (PaquetGameConnection*) pPaquet;

    // Reception de requete de connection a une partie deja cree

    // On va chercher la partie demandee
    Partie* wGame = GameManager::obtenirInstance()->getGame(wPaquet->getGameId());
    if(wGame)
    {
        if(wGame->requirePassword())
        {
            // Valider le mot de passe
            if(wGame->validatePassword(wPaquet->getPassword()))
            {
                // Mot de passe valide
                wPaquet->setConnectionState(connectPlayer(wPaquet->getUsername(), wGame));
            }
            else
            {
                wPaquet->setConnectionState(GAME_CONNECTION_WRONG_PASSWORD);
            }

        }
        else
        {
            // Pas besoin de mot de passe
            wPaquet->setConnectionState(connectPlayer(wPaquet->getUsername(), wGame));
        }

        // Si tout le monde connecte, on demarre la partie
        if(wGame->getGameStatus() == GAME_NOT_STARTED && wGame->obtenirJoueurGauche() && wGame->obtenirJoueurDroit())
        {
            GameManager::obtenirInstance()->startGame(wGame->getUniqueGameId(), "");
        }

    }
    else
    {
        wPaquet->setConnectionState(GAME_CONNECTION_GAME_NOT_FOUND);
    }

    

    GestionnaireReseau::obtenirInstance()->envoyerPaquet(wPaquet->getUsername(), wPaquet, TCP);

    return 0;
}


