//////////////////////////////////////////////////////////////////////////////
/// @file PartieSyncer.cpp
/// @author Mathieu Parent
/// @date 2013-03-13
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "PartieSyncer.h"
#include "..\Reseau\Paquets\PaquetMaillet.h"
#include "..\Reseau\GestionnaireReseau.h"
#include "RazerGameTypeDef.h"
#include "JoueurAbstrait.h"
#include "NoeudMaillet.h"
#include "NoeudRondelle.h"
#include "Partie.h"
#include "Terrain.h"
#include "GameManager.h"
#include "..\Reseau\Paquets\PaquetRondelle.h"


PartieSyncer::PartieSyncer( int pGameId, clock_t pFrequencyPerSec, SPJoueurAbstrait pPlayer1, SPJoueurAbstrait pPlayer2 )
{
    mLastTick = clock();
    mInterval = 1000/pFrequencyPerSec; // Arrondi au ms pres
    setPlayers(pPlayer1, pPlayer2);
    mGameId = pGameId;

    if(GestionnaireReseau::obtenirInstance()->getPlayerName() == "GameServer")
    {
        // Game server, on doit envoyer le paquet aux 2 joueurs
        if(pPlayer1)
        {
            if(pPlayer1->obtenirNom().length() != 0)
            {
                addDestinationIdentifier(pPlayer1->obtenirNom());
            }

            if(pPlayer2->obtenirNom().length() != 0)
            {
                addDestinationIdentifier(pPlayer2->obtenirNom());
            }
        }

    }
    else
    {
        // Client, on doit envoyer le paquet au GameServer uniquement
        addDestinationIdentifier("GameServer");
    }
}

PartieSyncer::~PartieSyncer( void )
{

}





bool PartieSyncer::isReadyToUpdate()
{
    clock_t wElapsed = clock()-mLastTick;
    if(wElapsed > mInterval)
    {
        mLastTick = clock();
        return true;
    }
    else
    {
        return false;
    }
}




void PartieSyncer::tick()
{
    if(isReadyToUpdate())
    {
        // Envoie des infos de maillet
        if(mPlayer1)
        {
            PaquetMaillet* wPaquet = (PaquetMaillet*) GestionnaireReseau::obtenirInstance()->creerPaquet(MAILLET);
            wPaquet->setPosition(mPlayer1->getControlingMallet()->getPosition());
            wPaquet->setEstAGauche(mPlayer1->getControlingMallet()->estAGauche());
            wPaquet->setGameId(mGameId);

            wPaquet->setNbAssociatedQueries(mDestinationIdentifiers.size());
            for(auto it=mDestinationIdentifiers.begin(); it!=mDestinationIdentifiers.end(); ++it)
            {
                GestionnaireReseau::obtenirInstance()->envoyerPaquet(*it, wPaquet, TCP);
            }
        }

        if(mPlayer2)
        {
            PaquetMaillet* wPaquet = (PaquetMaillet*) GestionnaireReseau::obtenirInstance()->creerPaquet(MAILLET);
            wPaquet->setPosition(mPlayer2->getControlingMallet()->getPosition());
            wPaquet->setEstAGauche(mPlayer2->getControlingMallet()->estAGauche());
            wPaquet->setGameId(mGameId);

            wPaquet->setNbAssociatedQueries(mDestinationIdentifiers.size());
            for(auto it=mDestinationIdentifiers.begin(); it!=mDestinationIdentifiers.end(); ++it)
            {
                GestionnaireReseau::obtenirInstance()->envoyerPaquet(*it, wPaquet, TCP);
            }
        }


        // Envoie des infos de rondelle

        Partie* wGame = GameManager::obtenirInstance()->getGame(mGameId);
        if(wGame)
        {
            NoeudRondelle* wPuck = wGame->getField()->getPuck();
            if(wPuck)
            {
                PaquetRondelle* wPaquet = (PaquetRondelle*) GestionnaireReseau::obtenirInstance()->creerPaquet(RONDELLE);

                wPaquet->setGameId(mGameId);
                wPaquet->setPosition(wPuck->getPosition());
                wPaquet->setVelocite(wPuck->obtenirVelocite());
                wPaquet->setVitesseRotation(wPuck->obtenirVitesseRotation());

                wPaquet->setNbAssociatedQueries(mDestinationIdentifiers.size());
                for(auto it=mDestinationIdentifiers.begin(); it!=mDestinationIdentifiers.end(); ++it)
                {
                    GestionnaireReseau::obtenirInstance()->envoyerPaquet(*it, wPaquet, TCP);
                }
            }
        }
        
    }



}



void PartieSyncer::setPlayers( SPJoueurAbstrait pPlayer1, SPJoueurAbstrait pPlayer2 )
{
    // On trouve le joueur network
    if(pPlayer1 && pPlayer1->obtenirType() != JOUEUR_NETWORK)
    {
        // Trouve
        // On le retire de la liste des destinationIdentifiers pour eviter a tout prix d'envoyer plusieurs fois le meme paquet
        if(mPlayer1)
        {
            for(auto it = mDestinationIdentifiers.begin(); it!=mDestinationIdentifiers.end(); ++it)
            {
                if((*it) == mPlayer1->obtenirNom())
                {
                    mDestinationIdentifiers.erase(it);
                    break;
                }
            }
        }


        mPlayer1 = pPlayer1;
        if(GestionnaireReseau::obtenirInstance()->getPlayerName() == "GameServer")
        {
            addDestinationIdentifier(pPlayer1->obtenirNom());
        }

    }

    if(pPlayer2 && pPlayer2->obtenirType() != JOUEUR_NETWORK)
    {
        // Trouve

        if(mPlayer2)
        {
            for(auto it = mDestinationIdentifiers.begin(); it!=mDestinationIdentifiers.end(); ++it)
            {
                if((*it) == mPlayer2->obtenirNom())
                {
                    mDestinationIdentifiers.erase(it);
                    break;
                }
            }
        }

        mPlayer2 = pPlayer2;
        if(GestionnaireReseau::obtenirInstance()->getPlayerName() == "GameServer")
        {
            addDestinationIdentifier(pPlayer2->obtenirNom());
        }
    }



}



void PartieSyncer::addDestinationIdentifier( const std::string& pIdentifier )
{
    mDestinationIdentifiers.push_back(pIdentifier);
}






///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


