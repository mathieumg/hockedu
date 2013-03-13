

#include "PaquetRunnableClientLourd.h"
#include "Paquets\PaquetMaillet.h"
#include "GameManager.h"
#include "Partie.h"

// A supprimmer apres le test
#include "JoueurAbstrait.h"
#include "NoeudMaillet.h"
#include <iostream>

#ifdef LINUX
#define _LARGE_TIME_API
#endif



int PaquetRunnable::RunnableMailletClient( Paquet* pPaquet )
{
    PaquetMaillet* wPaquet = (PaquetMaillet*) pPaquet;
    // Modification de la pos du maillet


    // Test
    Partie* wGame = GameManager::obtenirInstance()->getGame(wPaquet->getGameId());

    if(wGame)
    {
        Vecteur3 wPos = wPaquet->getPosition();
        wPos[VX] *= -1;
        // Mettre la position du maillet
        wGame->obtenirJoueurDroit()->getControlingMallet()->assignerPosSouris(wPos);
        wGame->obtenirJoueurDroit()->getControlingMallet()->updateObserver();
        std::cout << "PositionMaillet" << wPos << std::endl;

    }

    return 0;
}

