

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


#ifdef LINUX
#define _LARGE_TIME_API
#endif



int PaquetRunnable::RunnableMailletClient( Paquet* pPaquet )
{
    PaquetMaillet* wPaquet = (PaquetMaillet*) pPaquet;
    // Modification de la pos du maillet


    Partie* wGame = GameManager::obtenirInstance()->getGame(wPaquet->getGameId());

    if(wGame && wGame->getGameStatus() >= GAME_STARTED)
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
            JoueurAbstrait* wJoueur = maillet->obtenirJoueur();
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




int PaquetRunnable::RunnableGameConnectionClient( Paquet* pPaquet )
{
    PaquetGameConnection* wPaquet = (PaquetGameConnection*) pPaquet;

    // Reception de la reponse de connection, handling different dependant du state de retour
    switch(wPaquet->getConnectionState())
    {
    case GAME_CONNECTION_ACCEPTED_LEFT:
        {
            // Accepted, on est maintenant dans la partie demandee sur le serveur jeu a gauche
            int wGameId = GameManager::obtenirInstance()->addNewGame(SPJoueurHumain(new JoueurHumain(GestionnaireReseau::obtenirInstance()->getPlayerName())), SPJoueurNetwork(new JoueurNetwork()), true, wPaquet->getGameId());
            std::cout << "Connecte a la partie: " << wGameId << std::endl;
            FacadeModele::getInstance()->setProchainePartie(wGameId);
            break;
        }
    case GAME_CONNECTION_ACCEPTED_RIGHT:
        {
            // Accepted, on est maintenant dans la partie demandee sur le serveur jeu a droite
            int wGameId = GameManager::obtenirInstance()->addNewGame(SPJoueurNetwork(new JoueurNetwork()), SPJoueurHumain(new JoueurHumain(GestionnaireReseau::obtenirInstance()->getPlayerName())), true, wPaquet->getGameId());
            std::cout << "Connecte a la partie: " << wGameId << std::endl;
            FacadeModele::getInstance()->setProchainePartie(wGameId);
            break;
        }
    case GAME_CONNECTION_ALREADY_CONNECTED:
        {
            // User with this name already connected



            break;
        }
    case GAME_CONNECTION_GAME_FULL:
        {
            // Partie comporte deja 2 joueurs


        }
        // Dans tous les autres cas, la connection a echouee
    case GAME_CONNECTION_GAME_NOT_FOUND:
        {
            std::cout << "Game not found with id: " << wPaquet->getGameId() << std::endl;
            break;
        }
    case GAME_CONNECTION_WRONG_PASSWORD:
        {
            std::cout << "Wrong Password: " << std::endl;
            break;
        }
    case GAME_CONNECTION_REJECTED:
        {
            std::cout << "Connection rejected. No more info. Game: " << wPaquet->getGameId() << std::endl;
            break;
        }
    case GAME_CONNECTION_PENDING:
        {
            // Pas de break, meme comportement que default
        }

    default:
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

                if(wGame->getGameStatus() == GAME_PAUSED)
                {
                    // Resume game only

                }
                else if(wGame->getGameStatus() == GAME_READY)
                {
                    // Start game
                    // On utilise les noms de joueurs du paquet pour initialiser la partie

                    wGame->obtenirJoueurGauche()->modifierNom(wPaquet->getPlayer1Name());
                    wGame->obtenirJoueurDroit()->modifierNom(wPaquet->getPlayer2Name());

                    GameManager::obtenirInstance()->startGame(wPaquet->getGameId()); 
                }



                break;
            }
        case GAME_EVENT_PAUSE_GAME_USER_DISCONNECTED:
            {
                // L'autre joueur s'est deconnecte

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
                // Pas utilise en ce moment car le nb de points est fixe et le client va le detecter anyways

                break;
            }
        }
    }



    return 0;
}
