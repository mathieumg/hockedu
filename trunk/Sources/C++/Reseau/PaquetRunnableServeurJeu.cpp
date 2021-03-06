

#include "PaquetRunnableServeurJeu.h"

#ifdef LINUX
#define _LARGE_TIME_API
#endif

#include <time.h>
#include <sstream>
#include <iomanip>
#include <stdexcept>

#include "RelayeurMessage.h"
#include "Paquets\PaquetUserStatus.h"
#include "Paquets\PaquetGameStatus.h"
#include "Paquets\PaquetMaillet.h"
#include "Paquets\PaquetGameCreation.h"
#include "GameManager.h"
#include "Paquets\PaquetGameConnection.h"
#include "RazerGameTypeDef.h"
#include "PlayerNetworkServer.h"
#include "Runnable.h"
#include "NoeudMaillet.h"
#include "GestionnaireReseau.h"
#include "ControllerServeurJeu.h"
#include "PlayerComputer.h"
#include "Paquets\PaquetGameEvent.h"
#include "FacadeServeurJeu.h"
#include <iosfwd>
#include "Partie.h"
#include "Terrain.h"
#include "NoeudRondelle.h"

SINGLETON_DECLARATION_CPP(PaquetRunnableServeurJeuHelper);

/// ***** PAR CONVENTION, METTRE Game A LA FIN DU NOM DES DELEGATES


int PaquetRunnable::RunnableChatMessageServerGame( Paquet* pPaquet )
{
    PacketDataChatMessage* wData = (PacketDataChatMessage*) pPaquet->getData();


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


    wTimeOutput << "[" << wData->mOrigin << "]: " << wData->mMessage << std::endl;
    std::cout << wTimeOutput.str();

    // On veut relayer le message a une personne en particulier ou a tout le monde
    // On ne call donc pas delete dessus tout suite
    if(wData->mIsTargetGroup)
    {
        // On l'envoie a la personne dans groupName seulement
        RelayeurMessage::obtenirInstance()->relayerPaquet(wData->mGroupName, pPaquet, TCP);
    }
    else
    {
        // On envoie a tout le monde
        std::set<std::string> wListeAIgnorer;
        wListeAIgnorer.insert("MasterServer");
        RelayeurMessage::obtenirInstance()->relayerPaquetGlobalement(pPaquet, &wListeAIgnorer, TCP);
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
	// Modification de la pos du maillet
	Partie* wGame = GameManager::obtenirInstance()->getGame(wPaquet->getGameId());

	if(wGame && wGame->getGameStatus() == GAME_STARTED)
	{
		Vecteur3 wPos = wPaquet->getPosition();
		SPPlayerAbstract wJoueur;
		if(wPaquet->getEstAGauche())
		{
			wJoueur = wGame->obtenirJoueurGauche();
		}
		else
		{
			wJoueur = wGame->obtenirJoueurDroit();
		}

		if(wJoueur)
		{
			if(wJoueur->obtenirType() == JOUEUR_NETWORK_SERVEUR)
			{
				auto wNetworkPlayer = std::static_pointer_cast<PlayerNetworkServer>(wJoueur);
				wNetworkPlayer->setTargetDestination(wPos);
			}
		}
	}

    return 0;
}


void CallbackMapDownloaded(std::string pMapFilepath)
{
    if(pMapFilepath.find(".") == std::string::npos)
    {
        checkf(0);
        // On doit terminer la partie, le parametre est le map id
        FacadePortability::takeMutex(PaquetRunnableServeurJeuHelper::obtenirInstance()->mMutexMapMapnameGameId);
        std::list<int> wListe = PaquetRunnableServeurJeuHelper::obtenirInstance()->mMapMapnameGameId[pMapFilepath];
        for(auto it = wListe.begin(); it!=wListe.end(); ++it)
        {
            GameManager::obtenirInstance()->removeGame(*it);
        }
        wListe.empty();
        FacadePortability::releaseMutex(PaquetRunnableServeurJeuHelper::obtenirInstance()->mMutexMapMapnameGameId);
        return;
    }
    FacadePortability::takeMutex(PaquetRunnableServeurJeuHelper::obtenirInstance()->mMutexMapMapnameGameId);
    // A la reception, on peut mettre le field de tous les terrains qui attendaient pour ce fichier
    std::string wMapId = pMapFilepath.substr(pMapFilepath.find_last_of("\\")+1);
    wMapId = wMapId.substr(0, wMapId.find("."));
    std::list<int> wListe = PaquetRunnableServeurJeuHelper::obtenirInstance()->mMapMapnameGameId[wMapId];
    for(auto it = wListe.begin(); it!=wListe.end(); ++it)
    {
        Partie* wGame = GameManager::obtenirInstance()->getGame(*it);
        // checkf(wGame);
        // Pas de checkf car possible qu'une game precedente se soit terminee, on ne fait rien
        if(wGame)
        {
            wGame->setFieldName(pMapFilepath);
        }
    }
    wListe.empty();
    FacadePortability::releaseMutex(PaquetRunnableServeurJeuHelper::obtenirInstance()->mMutexMapMapnameGameId);
}

int PaquetRunnable::RunnableGameCreationServerGame( Paquet* pPaquet )
{
    PaquetGameCreation* wPaquet = (PaquetGameCreation*) pPaquet;
    
    // Demande de creation d'une nouvelle partie.

    // On regarde d'abord si une partie avec ce nom existe deja
    if(!GameManager::obtenirInstance()->getGame(wPaquet->getGameName()))
    {
        // On peut creer la partie
        int wGameId = GameManager::obtenirInstance()->addNewGame(GAME_TYPE_NETWORK_SERVER, Partie::POINTAGE_GAGNANT,SPPlayerAbstract(0), SPPlayerAbstract(0), true);
        //GameManager::obtenirInstance()->setMapForGame(wGameId, wPaquet->getMapName());
        Partie* wGame = GameManager::obtenirInstance()->getGame(wGameId);
        wGame->setName(wPaquet->getGameName());
        wGame->setPassword(wPaquet->getPassword());

        // On demarre le download de la map, c'est lui qui va set le fieldName une fois recu
        // Avant on sauvegarde la liaison entre le nom de la map et le id de la partie qui en a besoin
        FacadePortability::takeMutex(PaquetRunnableServeurJeuHelper::obtenirInstance()->mMutexMapMapnameGameId);
        std::stringstream ss;
        ss << wPaquet->getMapId();
        PaquetRunnableServeurJeuHelper::obtenirInstance()->mMapMapnameGameId[ss.str()].push_back(wGameId);
        FacadePortability::releaseMutex(PaquetRunnableServeurJeuHelper::obtenirInstance()->mMutexMapMapnameGameId);
        FacadeServeurJeu::getInstance()->downloadMap(0, wPaquet->getMapId(), CallbackMapDownloaded);
        

        // On peut meme utiliser le meme paquet pour renvoyer le message de confirmation
        wPaquet->setGameId(wGameId);

    }
    else
    {
        // Une partie avec ce nom existe deja, on renvoie un message pour dire que la creation a echouee
        // Si creation a ecouee, id = -1

        // On peut meme utiliser le meme paquet pour renvoyer le message de confirmation
        wPaquet->setGameId(-2);
    }

    wPaquet->setServerId(((ControllerServeurJeu*)GestionnaireReseau::obtenirInstance()->getController())->getServerId());
    GestionnaireReseau::obtenirInstance()->envoyerPaquet("MasterServer", wPaquet, TCP);

    return 0;
}



GameConnectionState connectPlayer(const std::string& pPlayerName, Partie* pGame)
{
    if(pGame)
    {
        if(pGame->obtenirNomJoueurGauche() == pPlayerName)
        {
            return GAME_CONNECTION_ACCEPTED_LEFT;
        }
        else if(pGame->obtenirNomJoueurDroit() == pPlayerName)
        {
            return GAME_CONNECTION_ACCEPTED_RIGHT;
        }


        if(!pGame->obtenirJoueurGauche())
        {
            // Pas de joueur gauche. On assigne le joueur la
            pGame->assignerJoueur(SPPlayerNetworkServer(new PlayerNetworkServer(pPlayerName)));
            return GAME_CONNECTION_ACCEPTED_LEFT;
        }
        else if(!pGame->obtenirJoueurDroit())
        {
            // Pas de joueur droit. On assigne le joueur la
            pGame->assignerJoueur(SPPlayerNetworkServer(new PlayerNetworkServer(pPlayerName)));
            return GAME_CONNECTION_ACCEPTED_RIGHT;
        }
        else if(pGame->obtenirJoueurGauche()->obtenirType() == JOUEUR_VIRTUEL)
        {
            pGame->modifierJoueurGauche(SPPlayerNetworkServer(new PlayerNetworkServer(pPlayerName)));
            pGame->reloadControleMallet();
            return GAME_CONNECTION_ACCEPTED_LEFT;
        }
        else if(pGame->obtenirJoueurDroit()->obtenirType() == JOUEUR_VIRTUEL)
        {
            pGame->modifierJoueurDroit(SPPlayerNetworkServer(new PlayerNetworkServer(pPlayerName)));
            pGame->reloadControleMallet();
            return GAME_CONNECTION_ACCEPTED_RIGHT;
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
    int wGameId = wPaquet->getGameId();
    Runnable* r = new Runnable([wGameId, wPaquet](Runnable*){
        // On va chercher la partie demandee
        Partie* wGame = GameManager::obtenirInstance()->getGame(wPaquet->getGameId());
        bool wSendStartGamePaquet = false;
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
                    wSendStartGamePaquet = true;
                }
                catch(ExceptionJeu& e)
                {
                    utilitaire::afficherErreur(e.what());
                    return;
                }
            }

            // Si tout le monde connecte, met la partie prete
            else if(wGame->getGameStatus() == GAME_NOT_READY && wGame->obtenirJoueurGauche() && wGame->obtenirJoueurDroit())
            {
                GameManager::obtenirInstance()->getGameReady(wGame->getUniqueGameId());
            }
        }
        else
        {
            wPaquet->setConnectionState(GAME_CONNECTION_GAME_NOT_FOUND);
        }
    
        GameConnectionState wConnState = wPaquet->getConnectionState();
        std::string wUsername = wPaquet->getUsername();
        float wTemps = wGame->obtenirTempsJeu();
        wPaquet->setGameTime(wTemps);
        GestionnaireReseau::obtenirInstance()->envoyerPaquet(wPaquet->getUsername(), wPaquet, TCP);
        

        
        
        if(wConnState != GAME_CONNECTION_ACCEPTED_LEFT && wConnState!=GAME_CONNECTION_ACCEPTED_RIGHT)
        {
            GestionnaireReseau::obtenirInstance()->removeSocket(wUsername, TCP);
        }
        /*else
        {
            PaquetGameEvent* wPaquetEventGameStart = (PaquetGameEvent*) UsinePaquet::creerPaquet(GAME_EVENT);
            wPaquetEventGameStart->setGameId(wPaquet->getGameId());
            wPaquetEventGameStart->setEvent(GAME_EVENT_START_GAME);
            wPaquetEventGameStart->setPlayer1Name(wGame->obtenirNomJoueurGauche());
            wPaquetEventGameStart->setPlayer2Name(wGame->obtenirNomJoueurDroit());
            RelayeurMessage::obtenirInstance()->relayerPaquetGame(wPaquet->getGameId(), wPaquetEventGameStart, TCP);
        }*/

    });
    RazerGameUtilities::RunOnUpdateThread(r,true);
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
                PlayerNetworkServer* wPlayerVise;
                PlayerNetworkServer* wPlayer2;
                if(wPaquet->getEventOnPlayerLeft())
                {
                    wPlayerVise = (PlayerNetworkServer*) wGame->obtenirJoueurGauche().get();
                    wPlayer2    = (PlayerNetworkServer*) wGame->obtenirJoueurDroit().get();
                }
                else
                {
                    wPlayerVise = (PlayerNetworkServer*) wGame->obtenirJoueurDroit().get();
                    wPlayer2    = (PlayerNetworkServer*) wGame->obtenirJoueurGauche().get();
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
                        PaquetGameEvent* wPaquetEventGameStart = (PaquetGameEvent*) UsinePaquet::creerPaquet(GAME_EVENT);
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

                if(wGame->getGameStatus() == GAME_STARTED && wGame->obtenirJoueurDroit() && wGame->obtenirJoueurDroit()->obtenirType() != JOUEUR_VIRTUEL && wGame->obtenirJoueurGauche() && wGame->obtenirJoueurGauche()->obtenirType() != JOUEUR_VIRTUEL)
                {
                    wGame->modifierEnPause(true);

                    PaquetGameEvent* wPaquetResponse = (PaquetGameEvent*) UsinePaquet::creerPaquet(GAME_EVENT);
                    wPaquetResponse->setGameId(wPaquet->getGameId());
                    wPaquetResponse->setEvent(GAME_EVENT_PAUSE_GAME_SIGNAL);
                    RelayeurMessage::obtenirInstance()->relayerPaquetGame(wPaquetResponse->getGameId(), wPaquetResponse, TCP);
                }

                break;
            }
        case GAME_EVENT_RESET_PUCK:
            {
                // Client demande de refaire la mise au jeu
                // On doit verifier que la rondelle ne bouge pas ou presque pas avant de lancer l'event
                Terrain* wField = wGame->getField();
                if(wField)
                {
                    float wSpeed = wField->getPuck()->obtenirVelocite().norme();
                    if(wSpeed < 320.0f)
                    {
                        // Vitesse assez basse
                        RelayeurMessage::obtenirInstance()->relayerPaquetGame(wGame->getUniqueGameId(), wPaquet, TCP);
                        int wGameId = wGame->getUniqueGameId();
                        Runnable* r = new Runnable([wGameId](Runnable*){
                            Partie* wGame = GameManager::obtenirInstance()->getGame(wGameId);
                            if(wGame)
                            {
                                wGame->miseAuJeu(false);
                            }
                        });
                        RazerGameUtilities::RunOnUpdateThread(r,true);
                    }
                }
                break;
            }
        case GAME_EVENT_PAUSE_GAME_USER_DISCONNECTED:
            {
                // Client se deconnecte, on doit le handle de la meme facon que si l'app crash
                int wGameId = wGame->getUniqueGameId();
                bool wPlayerLeft = wPaquet->getEventOnPlayerLeft();
                Runnable* r = new Runnable([wGameId, wPlayerLeft](Runnable*){
                    Partie* wGame = GameManager::obtenirInstance()->getGame(wGameId);
                    if(wGame)
                    {
                        // On va chercher le bon socket pour handle la disconnection dessus
                        SPSocket wSocket;
                        if(wPlayerLeft)
                        {
                            wSocket = GestionnaireReseau::obtenirInstance()->getSocket(wGame->obtenirNomJoueurGauche(), TCP);
                        }
                        else
                        {
                            wSocket = GestionnaireReseau::obtenirInstance()->getSocket(wGame->obtenirNomJoueurDroit(), TCP);
                        }
                        if(wSocket)
                        {
                            GestionnaireReseau::obtenirInstance()->getController()->handleDisconnectDetection(wSocket);
                            GestionnaireReseau::obtenirInstance()->removeSocket(wSocket);
                        }
                    }
                });
                RazerGameUtilities::RunOnUpdateThread(r,true);
                break;
            }
        case GAME_EVENT_ADD_AI:
            {
                // Client demande l'ajout d'un joueur AI
                int wGameId = wPaquet->getGameId();
                Runnable* r = new Runnable([wGameId, wPaquet](Runnable*){
                    Partie* wGame = GameManager::obtenirInstance()->getGame(wGameId);
                    bool wChanged = false;
                    if(wPaquet->getEventOnPlayerLeft() && wGame->obtenirNomJoueurDroit() != wPaquet->getPlayer1Name())
                    {
                        wGame->modifierJoueurDroit(SPPlayerComputer(new PlayerComputer("AITempDroit", 6, 50)));
                        wChanged = true;                   
                    }
                    else if(wGame->obtenirNomJoueurGauche() != wPaquet->getPlayer1Name())
                    {
                        wGame->modifierJoueurGauche(SPPlayerComputer(new PlayerComputer("AITempGauche", 6, 50)));
                        wChanged = true;
                    }

                    if(wChanged)
                    {
                        // Si joueur virtuel
                        if(wGame->getGameStatus() == GAME_NOT_READY)
                        {
                            GameManager::obtenirInstance()->getGameReady(wGameId);
                        }
                        else
                        {
                            wGame->reloadControleMallet();
                        }
                        wGame->setGameStatus(GAME_STARTED);

                        PaquetGameEvent* wPaquetStartGame = (PaquetGameEvent*) UsinePaquet::creerPaquet(GAME_EVENT);
                        wPaquetStartGame->setEvent(GAME_EVENT_START_GAME);
                        wPaquetStartGame->setGameId(wGameId);
                        wPaquetStartGame->setPlayer1Name(wGame->obtenirNomJoueurGauche());
                        wPaquetStartGame->setPlayer2Name(wGame->obtenirNomJoueurDroit());
                        RelayeurMessage::obtenirInstance()->relayerPaquetGame(wGameId, wPaquetStartGame, TCP);
                    }
                    

                });
                RazerGameUtilities::RunOnUpdateThread(r,true);
                break;
            }
        }
    }



    return 0;
}

PaquetRunnableServeurJeuHelper::PaquetRunnableServeurJeuHelper()
{
    FacadePortability::createMutex(mMutexMapMapnameGameId);
}
