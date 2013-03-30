

#include "PaquetRunnableServeurJeu.h"

#ifdef LINUX
#define _LARGE_TIME_API
#endif

#include <time.h>
#include <sstream>
#include <iomanip>
#include <stdexcept>

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
#include "GestionnaireReseau.h"
#include "ControllerServeurJeu.h"
#include "JoueurVirtuel.h"
#include "Paquets\PaquetGameEvent.h"

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

    if(wGame && wGame->getGameStatus() == GAME_RUNNING)
    {
        if(wGame->getGameStatus() == GAME_PAUSED)
        {
            // Si la partie est en pause, on doit dire au client de se mettre en pause (cas ou il n'aurait pas recu le paquet de mise en pause et il continue a envoyer une position)
            /*PaquetGameEvent* wPaquetResponse = (PaquetGameEvent*) GestionnaireReseau::obtenirInstance()->creerPaquet(GAME_EVENT);
            wPaquetResponse->setGameId(wPaquet->getGameId());
            wPaquetResponse->setEvent(GAME_EVENT_PAUSE_GAME_SIGNAL);
            RelayeurMessage::obtenirInstance()->relayerPaquetGame(wPaquetResponse->getGameId(), wPaquetResponse, TCP);*/
        }
        else if(wGame->getGameStatus() != GAME_NOT_READY)
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
        int wGameId = GameManager::obtenirInstance()->addNewGame(GAME_TYPE_NETWORK_SERVER,SPJoueurAbstrait(0), SPJoueurAbstrait(0), true);
        //GameManager::obtenirInstance()->setMapForGame(wGameId, wPaquet->getMapName());
        Partie* wGame = GameManager::obtenirInstance()->getGame(wGameId);
        wGame->setName(wPaquet->getGameName());
        wGame->setFieldName(wPaquet->getMapName());

        // On peut meme utiliser le meme paquet pour renvoyer le message de confirmation
        wPaquet->setGameId(wGameId);

        // If not local server -> Reply to Master Server.
        if(!ControllerServeurJeu::isLocalServer())
        {
            wPaquet->setServerId(((ControllerServeurJeu*)GestionnaireReseau::obtenirInstance()->getController())->getServerId());
            GestionnaireReseau::obtenirInstance()->envoyerPaquet("MasterServer", wPaquet, TCP);
        }
        else // Otherwise, the user is already connected to the server, so we reply directly to him.
        {
            GestionnaireReseau::obtenirInstance()->envoyerPaquet(wPaquet->getUsername(), wPaquet, TCP);
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

        // Dans le cas ou la partie est en pause, c'est que un des 2 joueurs s'est deconnected et il est en train de se reconnecter. Il faut lui reassigner son maillet sans reset la partie
        if(wGame->getGameStatus() == GAME_PAUSED && (wPaquet->getConnectionState() == GAME_CONNECTION_ACCEPTED_LEFT || wPaquet->getConnectionState() == GAME_CONNECTION_ACCEPTED_RIGHT))
        {
            
            try
            {
                wGame->reloadControleMallet();
                wGame->modifierEnPause(false);
            }
            catch(ExceptionJeu& e)
            {
                utilitaire::afficherErreur(e.what());
                return false;
            }
        }

        // Si tout le monde connecte, on demarre la partie
        else if(wGame->getGameStatus() == GAME_NOT_READY && wGame->obtenirJoueurGauche() && wGame->obtenirJoueurDroit())
        {
            GameManager::obtenirInstance()->getGameReady(wGame->getUniqueGameId());
        }
    }
    else
    {
        wPaquet->setConnectionState(GAME_CONNECTION_GAME_NOT_FOUND);
    }
    
    GestionnaireReseau::obtenirInstance()->envoyerPaquet(wPaquet->getUsername(), wPaquet, TCP);

    return 0;
}



int PaquetRunnable::RunnableGameEventServerGame( Paquet* pPaquet )
{
    PaquetGameEvent* wPaquet = (PaquetGameEvent*) pPaquet;

    Partie* wGame = GameManager::obtenirInstance()->getGame(wPaquet->getGameId());
    if(wGame)
    {
        switch(wPaquet->getEvent())
        {
        case GAME_EVENT_START_GAME:
            {
                // Demande de unpause, on doit verifier si 2 joueurs sont biens connectes

                if(GestionnaireReseau::obtenirInstance()->getSocket(wGame->obtenirNomJoueurGauche(), TCP) && GestionnaireReseau::obtenirInstance()->getSocket(wGame->obtenirNomJoueurDroit(), TCP))
                {
                    wGame->modifierEnPause(false);
                    RelayeurMessage::obtenirInstance()->relayerPaquetGame(wPaquet->getGameId(), wPaquet, TCP);
                }

                break;
            }
        case GAME_EVENT_READY:
            {
                // Envoyer par un client pour signaler qu'il est pret a demarrer la partie
                JoueurNetworkServeur* wPlayerVise;
                JoueurNetworkServeur* wPlayer2;
                if(wPaquet->getEventOnPlayerLeft())
                {
                    wPlayerVise = (JoueurNetworkServeur*) wGame->obtenirJoueurGauche().get();
                    wPlayer2    = (JoueurNetworkServeur*) wGame->obtenirJoueurDroit().get();
                }
                else
                {
                    wPlayerVise = (JoueurNetworkServeur*) wGame->obtenirJoueurDroit().get();
                    wPlayer2    = (JoueurNetworkServeur*) wGame->obtenirJoueurGauche().get();
                }

                if(wPlayerVise)
                {
                    wPlayerVise->setReady(true);
                }
                
                // Verifier si les 2 joueurs sont connectes, si oui, on envoie le paquet de debut de partie
                if(wPlayerVise && wPlayer2)
                {
                    if(wPlayer2->isReady())
                    {
                        // On envoie le paquet
                        PaquetGameEvent* wPaquetEventGameStart = (PaquetGameEvent*) GestionnaireReseau::obtenirInstance()->creerPaquet(GAME_EVENT);
                        wPaquetEventGameStart->setGameId(wPaquet->getGameId());
                        wPaquetEventGameStart->setEvent(GAME_EVENT_START_GAME);
                        wPaquetEventGameStart->setPlayer1Name(wGame->obtenirNomJoueurGauche());
                        wPaquetEventGameStart->setPlayer2Name(wGame->obtenirNomJoueurDroit());

                        RelayeurMessage::obtenirInstance()->relayerPaquetGame(wPaquet->getGameId(), wPaquetEventGameStart, TCP);

                        // On demarre aussi la partie localement
                        GameManager::obtenirInstance()->startGame(wGame->getUniqueGameId());
                    }
                }



                break;
            }
        case GAME_EVENT_PAUSE_GAME_REQUESTED:
            {
                // Client demande une pause

                if(wGame->getGameStatus() == GAME_RUNNING || wGame->getGameStatus() == GAME_STARTED)
                {
                    wGame->modifierEnPause(true);

                    PaquetGameEvent* wPaquetResponse = (PaquetGameEvent*) GestionnaireReseau::obtenirInstance()->creerPaquet(GAME_EVENT);
                    wPaquetResponse->setGameId(wPaquet->getGameId());
                    wPaquetResponse->setEvent(GAME_EVENT_PAUSE_GAME_SIGNAL);
                    RelayeurMessage::obtenirInstance()->relayerPaquetGame(wPaquetResponse->getGameId(), wPaquetResponse, TCP);
                }

                break;
            }
        }
    }



    return 0;
}


////////// Section Bonus
int PaquetRunnable::RunnableBonusMailletMuretServerGame( PaquetBonus* pPaquet )
{
    PaquetBonusInfosMailletMurets* wInfos = (PaquetBonusInfosMailletMurets*) pPaquet->getPaquetInfos();



    return 0;
}


int PaquetRunnable::RunnableBonusGoalerServerGame( PaquetBonus* pPaquet )
{
    PaquetBonusInfosGoaler* wInfos = (PaquetBonusInfosGoaler*) pPaquet->getPaquetInfos();



    return 0;
}
