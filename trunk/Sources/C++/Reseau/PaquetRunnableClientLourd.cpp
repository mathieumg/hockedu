

#include "PaquetRunnableClientLourd.h"
#include "Paquets\PaquetMaillet.h"
#include "GameManager.h"
#include "Partie.h"

// A supprimmer apres le test
#include "JoueurAbstrait.h"
#include "NoeudMaillet.h"
#include "NoeudRondelle.h"
#include <iostream>
#include <intrin.h>
#include "Paquets\PaquetRondelle.h"
#include "Runnable.h"
#include "NoeudRondelle.h"
#include "Terrain.h"
#include "Paquets\PaquetGameConnection.h"
#include "RazerGameTypeDef.h"
#include "JoueurHumain.h"
#include "JoueurNetwork.h"
#include "FacadeModele.h"
#include "Paquets\PaquetGameEvent.h"
#include "SoundFMOD.h"
#include "Box2D\Common\b2Math.h"
#include "Box2D\Dynamics\b2Body.h"
#include "GestionnaireHUD.h"
#include "ObjetsGlobaux\PartieServeurs.h"
#include "Paquets\PaquetPortal.h"
#include "Paquets\PaquetGameCreation.h"
#include "VisiteurCollision.h"
#include "NodeBonus.h"


#ifdef LINUX
#define _LARGE_TIME_API
#endif



int PaquetRunnable::RunnableMailletClient( Paquet* pPaquet )
{
    PaquetMaillet* wPaquet = (PaquetMaillet*) pPaquet;
    // Modification de la pos du maillet


    Partie* wGame = GameManager::obtenirInstance()->getGame(wPaquet->getGameId());

    if(wGame && wGame->getGameStatus() == GAME_STARTED)
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
        // Si maillet pas encore cree (ex: transition vers le mode jeu en cours et deja reception de paquets)
        if(maillet)
        {
            SPJoueurAbstrait wJoueur = maillet->obtenirJoueur();
            if(wJoueur->obtenirType() == JOUEUR_NETWORK)
            {
                int wGameId = wPaquet->getGameId();
                Runnable* r = new Runnable([wGameId,maillet,wPos](Runnable*){
                    if(GameManager::obtenirInstance()->getGame(wGameId))
                    {
                        // Mettre la position du maillet
                        maillet->setTargetDestination(wPos);
                    }
                    

                });
                //maillet->attach(r);
                RazerGameUtilities::RunOnUpdateThread(r,true);
            }
        }
        
    }

    return 0;
}



int PaquetRunnable::RunnableRondelleClient( Paquet* pPaquet )
{
    PaquetRondelle* wPaquet = (PaquetRondelle*) pPaquet;
    // Modification des infos de la rondelle


    Partie* wGame = GameManager::obtenirInstance()->getGame(wPaquet->getGameId());

    if(wGame)
    {
        Terrain* wField = wGame->getField();
        if(wField && wField->isInit())
        {
            NoeudRondelle* wPuck = wGame->getField()->getPuck();
            if(wPuck)
            {
                Vecteur3 wPos = wPaquet->getPosition();
                Vecteur3 wVelocite = wPaquet->getVelocite();
                float wVitesseRot = wPaquet->getVitesseRotation();
                int wGameId = wPaquet->getGameId();
                Runnable* r = new Runnable([wGameId, wPuck, wPos, wVelocite, wVitesseRot](Runnable*){
                    if(wPuck && GameManager::obtenirInstance()->getGame(wGameId))
                    {
                        wPuck->modifierVitesseRotation(wVitesseRot);
                        wPuck->setPosition(wPos);
                        wPuck->modifierVelocite(wVelocite);
                    }
                });
                //wPuck->attach(r);
                RazerGameUtilities::RunOnUpdateThread(r,true);
            }
        }
    }

    return 0;
}




int PaquetRunnable::RunnableGameCreationClient( Paquet* pPaquet )
{
    PaquetGameCreation* wPaquet = (PaquetGameCreation*) pPaquet;

    if(wPaquet->getGameId() == -1)
    {
        // Creation a echouee
        std::cout << "Creation de partie echouee" << std::endl;
		GestionnaireReseau::obtenirInstance()->transmitEvent(GAME_CREATION_FAILED);
    }
    else
    {
        // Creation reussie
        std::cout << "Creation de partie reussie" << std::endl;

        GestionnaireReseau::obtenirInstance()->demarrerNouvelleConnection("GameServer", wPaquet->getServerIP(), TCP);
        SPSocket wSocketGameServer = GestionnaireReseau::obtenirInstance()->getSocket("GameServer", TCP);

        //GestionnaireReseau::obtenirInstance()->demarrerNouvelleConnection("GameServer", wPaquet->getServerIP(), UDP);

        PaquetGameConnection* wPaquetConnexion = (PaquetGameConnection*) GestionnaireReseau::obtenirInstance()->creerPaquet(GAME_CONNECTION);
        wPaquetConnexion->setGameId(wPaquet->getGameId());
        wPaquetConnexion->setGameServerId(wPaquet->getServerId());
        wPaquetConnexion->setPassword(FacadeModele::getInstance()->getGameCreationPassword());

        wSocketGameServer->setOnConnectionCallback([wPaquetConnexion]()->void{ GestionnaireReseau::obtenirInstance()->envoyerPaquet("GameServer", wPaquetConnexion, TCP); });

    }

    return 0;
}



int PaquetRunnable::RunnableGameConnectionClient( Paquet* pPaquet )
{
    PaquetGameConnection* wPaquet = (PaquetGameConnection*) pPaquet;

    // Reception de la reponse de connection, handling different dependant du state de retour
    switch(wPaquet->getConnectionState())
    {
    case GAME_CONNECTION_ACCEPTED_LEFT:
        {
            // Accepted, on est maintenant dans la partie demandee sur le serveur jeu a gauche
            int wGameId = GameManager::obtenirInstance()->addNewGame(GAME_TYPE_NETWORK_CLIENT,SPJoueurHumain(new JoueurHumain(GestionnaireReseau::obtenirInstance()->getPlayerName())), SPJoueurNetwork(new JoueurNetwork()), true, wPaquet->getGameId());
            std::cout << "Connecte a la partie: " << wGameId << std::endl;
            FacadeModele::getInstance()->setProchainePartie(wGameId);
            GestionnaireReseau::obtenirInstance()->transmitEvent(GAME_CONNECTION_RESPONSE_SUCCESS);
            break;
        }
    case GAME_CONNECTION_ACCEPTED_RIGHT:
        {
            // Accepted, on est maintenant dans la partie demandee sur le serveur jeu a droite
            int wGameId = GameManager::obtenirInstance()->addNewGame(GAME_TYPE_NETWORK_CLIENT,SPJoueurNetwork(new JoueurNetwork()), SPJoueurHumain(new JoueurHumain(GestionnaireReseau::obtenirInstance()->getPlayerName())), true, wPaquet->getGameId());
            std::cout << "Connecte a la partie: " << wGameId << std::endl;
            FacadeModele::getInstance()->setProchainePartie(wGameId);
            GestionnaireReseau::obtenirInstance()->transmitEvent(GAME_CONNECTION_RESPONSE_SUCCESS);
            break;
        }
    case GAME_CONNECTION_ALREADY_CONNECTED:
        {
            // User with this name already connected
            GestionnaireReseau::obtenirInstance()->transmitEvent(GAME_CONNECTION_RESPONSE_ALREADY_CONNECTED);


            break;
        }
    case GAME_CONNECTION_GAME_FULL:
        {
            // Partie comporte deja 2 joueurs
            GestionnaireReseau::obtenirInstance()->transmitEvent(GAME_CONNECTION_RESPONSE_GAME_FULL);
            break;
        }
        // Dans tous les autres cas, la connection a echouee
    case GAME_CONNECTION_GAME_NOT_FOUND:
        {
            GestionnaireReseau::obtenirInstance()->transmitEvent(GAME_CONNECTION_RESPONSE_GAME_NOT_FOUND);
            std::cout << "Game not found with id: " << wPaquet->getGameId() << std::endl;
            break;
        }
    case GAME_CONNECTION_WRONG_PASSWORD:
        {
            GestionnaireReseau::obtenirInstance()->transmitEvent(GAME_CONNECTION_RESPONSE_WRONG_PASSWORD_CS);
            std::cout << "Wrong Password: " << std::endl;
            break;
        }
    case GAME_CONNECTION_REJECTED:
        {
            GestionnaireReseau::obtenirInstance()->transmitEvent(GAME_CONNECTION_RESPONSE_GAME_CONNECTION_GENERAL_FAILURE);
            std::cout << "Connection rejected. No more info. Game: " << wPaquet->getGameId() << std::endl;
            break;
        }
    case GAME_CONNECTION_PENDING:
        {
            // Pas de break, meme comportement que default
        }
    case GAME_CONNECTION_REPLY_GAME_SERVER_IP:
        {
            // Reponse du serveur maitre pour nous donner l'ip du server maitre
            GestionnaireReseau* wGestionnaireReseau = GestionnaireReseau::obtenirInstance();
            wPaquet->setConnectionState(GAME_CONNECTION_PENDING);
            wPaquet->setUsername(wGestionnaireReseau->getPlayerName());

            wGestionnaireReseau->demarrerNouvelleConnection("GameServer", wPaquet->getGameServerIp(), TCP);

            SPSocket wSocketGameServer = wGestionnaireReseau->getSocket("GameServer", TCP);
            wSocketGameServer->setOnConnectionCallback([wGestionnaireReseau, wPaquet]()->void {
                // Envoie de la requete de connexion au serveur jeu
                wGestionnaireReseau->envoyerPaquet("GameServer", wPaquet, TCP);
            });
            break;
        }
    default:
        GestionnaireReseau::obtenirInstance()->transmitEvent(GAME_CONNECTION_RESPONSE_GAME_CONNECTION_GENERAL_FAILURE);
        std::cout << "Error occured connecting to game: " << wPaquet->getGameId() << std::endl;
        // State invalide, on ne fait rien
        break;
    }



    return 0;
}



int PaquetRunnable::RunnableGameEventClient( Paquet* pPaquet )
{
    PaquetGameEvent* wPaquet = (PaquetGameEvent*) pPaquet;

    Partie* wGame = GameManager::obtenirInstance()->getGame(wPaquet->getGameId());

    if(wGame)
    {
        switch(wPaquet->getEvent())
        {
        case GAME_EVENT_START_GAME:
            {
                // Serveur dit de commencer la partie (les 2 clients sont prets)
                // Aussi utilise pour unpause
                int wGameId = wPaquet->getGameId();
                Runnable* r = new Runnable([wGameId, wPaquet](Runnable*){
                    Partie* wGame = GameManager::obtenirInstance()->getGame(wGameId);
                    if(wGame->getGameStatus() == GAME_PAUSED)
                    {
                        // on reassigne le nom des joueurs au cas ou ils auraient change
                        wGame->obtenirJoueurGauche()->modifierNom(wPaquet->getPlayer1Name());
                        wGame->obtenirJoueurDroit()->modifierNom(wPaquet->getPlayer2Name());
                        // Resume game only
                        GestionnaireHUD::obtenirInstance()->setForeverAloneVisibility(false);
                        wGame->modifierEnPause(false);
                    }
                    else if(wGame->getGameStatus() == GAME_READY)
                    {
                        // Start game
                        // On utilise les noms de joueurs du paquet pour initialiser la partie

                        wGame->obtenirJoueurGauche()->modifierNom(wPaquet->getPlayer1Name());
                        wGame->obtenirJoueurDroit()->modifierNom(wPaquet->getPlayer2Name());

                        GameManager::obtenirInstance()->startGame(wPaquet->getGameId()); 
                    }
                    GestionnaireReseau::obtenirInstance()->transmitEvent(GAME_SERVER_USER_CONNECTED);
                });
                RazerGameUtilities::RunOnUpdateThread(r,true);
                break;
            }
        case GAME_EVENT_PAUSE_GAME_USER_DISCONNECTED:
            {
                int wGameId = wPaquet->getGameId();
                Runnable* r = new Runnable([wGameId](Runnable*){
                    Partie* wGame = GameManager::obtenirInstance()->getGame(wGameId);
                    // L'autre joueur s'est deconnecte
                    if(wGame->getGameStatus() == GAME_STARTED)
                    {
                        wGame->modifierEnPause(true);
                        GestionnaireHUD::obtenirInstance()->setForeverAloneVisibility(true);
                    }
                    GestionnaireReseau::obtenirInstance()->transmitEvent(GAME_SERVER_USER_DISCONNECTED);
                    std::cout << "Other player disconnected" << std::endl;
                });
                RazerGameUtilities::RunOnUpdateThread(r,true);

                break;
            }
        case GAME_EVENT_PLAYER_SCORED:
            {
                // Pour s'assurer de bien compter les points meme avec des problemes de reseau. Annonce les buts
                // Attention de ne pas les faire compter par le client quand la physique detecte un but, sinon buts en double
                int wGameId = wPaquet->getGameId();
                bool wIsPlayerLeft = wPaquet->getEventOnPlayerLeft();
                Runnable* r = new Runnable([wGameId, wIsPlayerLeft](Runnable*){
                    Partie* wGameRunnable = GameManager::obtenirInstance()->getGame(wGameId);
                    if(wGameRunnable)
                    {
                        // Mettre event score
                        if(wIsPlayerLeft)
                        {
                            wGameRunnable->incrementerPointsJoueurGauche(true);
                        }
                        else
                        {
                            wGameRunnable->incrementerPointsJoueurDroit(true);
                        }

                        SoundFMOD::obtenirInstance()->playEffect(GOAL_EFFECT);
                        wGameRunnable->miseAuJeu();

                        if(wGameRunnable->getField())
                        {
                            NoeudRondelle* wPuck = wGameRunnable->getField()->getPuck();
                            if(wPuck)
                            {
                                wPuck->getPhysicBody()->SetLinearVelocity(b2Vec2(0,0));
                                wPuck->getPhysicBody()->SetAngularVelocity(0);
                                wPuck->setAngle(0);
                            }
                        }
                    }
                });
                RazerGameUtilities::RunOnUpdateThread(r,true);

                break;
            }
        case GAME_EVENT_GAME_ENDED:
            {
                // Fin de la partie
                GestionnaireReseau::obtenirInstance()->transmitEvent(GAME_ENDED_CS);
                break;
            }
        case GAME_EVENT_CHANGE_LAST_MALLET:
            {
                int wGameId = wPaquet->getGameId();
                bool wIsPlayerLeft = wPaquet->getEventOnPlayerLeft();
                Runnable* r = new Runnable([wGameId, wIsPlayerLeft](Runnable*){
                    // On doit mettre a jour le dernier maillet qui a touche a la rondelle pour eviter un crash dans la detection de collision avec les bonus
                    NoeudMaillet* wMaillet;
                    Partie* wGameRunnable = GameManager::obtenirInstance()->getGame(wGameId);
                    if(wIsPlayerLeft)
                    {
                        if(wGameRunnable->obtenirJoueurGauche())
                            wMaillet = wGameRunnable->obtenirJoueurGauche()->getControlingMallet();
                    }
                    else
                    {
                        if(wGameRunnable->obtenirJoueurDroit())
                            wMaillet = wGameRunnable->obtenirJoueurDroit()->getControlingMallet();
                    }

                    if(wGameRunnable->getField())
                    {
                        NoeudRondelle* wPuck = wGameRunnable->getField()->getPuck();
                        if(wPuck)
                        {
                            wPuck->setLastHittingMallet(wMaillet);
                        }
                    }
                });
                RazerGameUtilities::RunOnUpdateThread(r,true);
                break;
            }
        case GAME_EVENT_PAUSE_GAME_SIGNAL:
            {
                int wGameId = wGame->getUniqueGameId();
                Runnable* r = new Runnable([wGameId](Runnable*){
                    // Le serveur demande de se mettre en pause
                    // Si on est en cours de jeu, on pause
                    Partie* wGame = GameManager::obtenirInstance()->getGame(wGameId);
                    if(wGame->getGameStatus() == GAME_STARTED)
                    {
                        wGame->modifierEnPause(true);
                    }
                });
                RazerGameUtilities::RunOnUpdateThread(r,true);
                break;
            }
        case GAME_EVENT_RESET_PUCK:
            {
                // Le serveur nous dit de reset la puck (refaire une mise au jeu) apres avoir recu la request d'un des clients
                int wGameId = wGame->getUniqueGameId();
                Runnable* r = new Runnable([wGameId](Runnable*){
                    Partie* wGame = GameManager::obtenirInstance()->getGame(wGameId);
                    if(wGame)
                    {
                        wGame->miseAuJeu(false);
                    }
                });
                RazerGameUtilities::RunOnUpdateThread(r,true);
                break;
            }
        }
    }



    return 0;
}



int PaquetRunnable::RunnableBonusClient( Paquet* pPaquet )
{
    PaquetBonus* wPaquet = (PaquetBonus*)pPaquet;
    int wGameId = wPaquet->getGameId();
    PaquetBonusAction wAction = wPaquet->getBonusAction();
    BonusType wType = wPaquet->getBonusType();
    Vecteur2 pos = wPaquet->getBonusPosition();

    switch(wAction)
    {
    case BONUS_ACTION_SPAN :
        {
            Runnable* r = new Runnable([wGameId, pos, wType](Runnable*){

            Partie* wGame = GameManager::obtenirInstance()->getGame(wGameId);
            if(wGame)
            {
                auto field = wGame->getField();
                if(field)
                {
                    VisiteurCollision v(pos, false);
                    field->acceptVisitor(v);
                    ConteneurNoeuds cn;
                    v.obtenirListeCollision(cn);

                    for(auto iter = cn.begin(); iter != cn.end(); ++iter)
                    {
                        if((*iter)->getDefaultNodeKey() == RAZER_KEY_BONUS)
                        {
                            ((NodeBonus*)(*iter))->displayBonus(wType);
                        }
                    }
                }
            }
            });
            RazerGameUtilities::RunOnUpdateThread(r,true);
            break;
        }

    case BONUS_ACTION_EXECUTE :
        {
            Runnable* r = new Runnable([wGameId, pos, wType](Runnable*){

            Partie* wGame = GameManager::obtenirInstance()->getGame(wGameId);
            if(wGame)
            {
                auto field = wGame->getField();
                if(field)
                {
                    VisiteurCollision v(pos, false);
                    field->acceptVisitor(v);
                    ConteneurNoeuds cn;
                    v.obtenirListeCollision(cn);

                    for(auto iter = cn.begin(); iter != cn.end(); ++iter)
                    {
                        if((*iter)->getDefaultNodeKey() == RAZER_KEY_BONUS)
                        {
                            ((NodeBonus*)(*iter))->ExecuteBonus(field->getPuck());
                        }
                    }
                }
            }
            });
            RazerGameUtilities::RunOnUpdateThread(r,true);
            break;
        }

    case BONUS_ACTION_END :
        {
            Runnable* r = new Runnable([wGameId, pos](Runnable*){

            /*Partie* wGame = GameManager::obtenirInstance()->getGame(wGameId);
            if(wGame)
            {
                if(wGame->getField())
                {
                    wGame->getField()->getPuck()->setPosition(pos);
                }
            }*/
            });
            RazerGameUtilities::RunOnUpdateThread(r,true);
            break;
        }
    }

    return 0;
}



int PaquetRunnable::RunnablePortalClient( Paquet* pPaquet )
{
    PaquetPortal* wPaquet = (PaquetPortal*) pPaquet;
    int wGameId = wPaquet->getGameId();
    Vecteur3 pos = wPaquet->getPosition();
    Runnable* r = new Runnable([wGameId, pos](Runnable*){
        Partie* wGame = GameManager::obtenirInstance()->getGame(wGameId);
        if(wGame)
        {
            if(wGame->getField())
            {
                wGame->getField()->getPuck()->setPosition(pos);
            }
        }
    });
    RazerGameUtilities::RunOnUpdateThread(r,true);


    return 0;
}






