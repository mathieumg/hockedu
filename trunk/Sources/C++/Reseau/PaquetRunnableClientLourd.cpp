

#include "PaquetRunnableClientLourd.h"
#include "Paquets\PaquetMaillet.h"
#include "GameManager.h"
#include "Partie.h"

// A supprimmer apres le test
#include "JoueurAbstrait.h"
#include "NoeudMaillet.h"
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

                Runnable* r = new Runnable([maillet,wPos](Runnable*){

                    // Mettre la position du maillet
                    maillet->assignerPosSouris(wPos);

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

                Runnable* r = new Runnable([wPuck, wPos, wVelocite, wVitesseRot](Runnable*){
                    if(wPuck)
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