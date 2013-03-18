

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

#ifdef LINUX
#define _LARGE_TIME_API
#endif



int PaquetRunnable::RunnableMailletClient( Paquet* pPaquet )
{
    PaquetMaillet* wPaquet = (PaquetMaillet*) pPaquet;
    // Modification de la pos du maillet


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

        Runnable* r = new Runnable([maillet,wPos](Runnable*){
                
            // Mettre la position du maillet
            maillet->assignerPosSouris(wPos);

        });
        maillet->attach(r);
        RazerGameUtilities::RunOnUpdateThread(r,true);
        
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
        NoeudRondelle* wPuck = wGame->getField()->getPuck();
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
        wPuck->attach(r);
        RazerGameUtilities::RunOnUpdateThread(r,true);
    }

    return 0;
}

