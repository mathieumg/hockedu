#include "ControllerServeurJeu.h"
#include "../Reseau/GestionnaireReseau.h"
#include "../Reseau/PaquetRunnable.h"
#include "ExceptionsReseau/ExceptionReseauSocketDeconnecte.h"
#include "PaquetRunnableServeurJeu.h"
#include "../Application/GameManager.h"
#include "Partie.h"
#include "Paquets/PaquetGameEvent.h"
#include "RelayeurMessage.h"
#include "PaquetHandlers/PacketHandlerBonus.h"
#include "Runnable.h"
#include "RazerGameTypeDef.h"
#include "JoueurAbstrait.h"


bool ControllerServeurJeu::mIsLocalServer = false;


int CallbackGameEndedServeurJeu(int pGameId, GameStatus pNewGameStatus)
{
    if(pNewGameStatus == GAME_ENDED)
    {
        Partie* wGame = GameManager::obtenirInstance()->getGame(pGameId);

        if(wGame)
        {
            PaquetGameEvent* wPaquet = (PaquetGameEvent*) GestionnaireReseau::obtenirInstance()->creerPaquet(GAME_EVENT);

            wPaquet->setGameId(pGameId);
            wPaquet->setEvent(GAME_EVENT_GAME_ENDED);

            RelayeurMessage::obtenirInstance()->relayerPaquetGame(pGameId, wPaquet);

        }
    }
    return 0;
}


int CallbackSetPatieSyncerServeurJeu(int pGameId, GameStatus)
{
    Partie* wGame = GameManager::obtenirInstance()->getGame(pGameId);
    if(wGame)
    {
        wGame->getPartieSyncer()->addDestinationIdentifier(wGame->obtenirNomJoueurGauche());
        wGame->getPartieSyncer()->addDestinationIdentifier(wGame->obtenirNomJoueurDroit());
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn  ControllerServeurJeu::ControllerServeurJeu()
///
/// Constructeur
///
///
/// @return
///
////////////////////////////////////////////////////////////////////////
ControllerServeurJeu::ControllerServeurJeu()
{
    // Ajouter tous les Runnables dependant de ce qui est handled selon le type de controlleur
    mPaquetRunnables[EVENT]                         = PaquetRunnable::RunnableEvent;
    mPaquetRunnables[USER_STATUS]                   = PaquetRunnable::RunnableUserStatusServerGame;
    mPaquetRunnables[CHAT_MESSAGE]                  = PaquetRunnable::RunnableChatMessageServerGame;
    mPaquetRunnables[GAME_STATUS]                   = PaquetRunnable::RunnableGameStatusServerGame;
    mPaquetRunnables[GAME_CREATION_REQUEST]         = PaquetRunnable::RunnableGameCreationServerGame;
    mPaquetRunnables[GAME_CONNECTION]               = PaquetRunnable::RunnableGameConnectionServerGame;
    mPaquetRunnables[GAME_EVENT]                    = PaquetRunnable::RunnableGameEventServerGame;
    mPaquetRunnables[BONUS]                         = PaquetRunnable::RunnableBonus;
    mPaquetRunnables[MAILLET]                       = PaquetRunnable::RunnableMailletServerGame;

    // Runnables pour les bonus
    PacketHandlerBonus::mRunnableList[BONUS_MAILLET_MURETS] = PaquetRunnable::RunnableBonusMailletMuretServerGame;
    PacketHandlerBonus::mRunnableList[BONUS_GOALER]         = PaquetRunnable::RunnableBonusMailletMuretServerGame;

    GameManager::obtenirInstance()->addGameUpdateCallback(CallbackSetPatieSyncerServeurJeu);
    GameManager::obtenirInstance()->addGameUpdateCallback(CallbackGameEndedServeurJeu);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void ControllerServeurJeu::handleEvent( int pEventCode, va_list pListeElems )
///
/// Reception d'un événement provenant du réseau
///
/// @param[in] int pEventCode
/// @param[in] va_list pListeElems
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ControllerServeurJeu::handleEvent( EventCodes pEventCode, va_list pListeElems )
{
    switch(pEventCode) {
    case USER_DISCONNECTED:
        {
            std::string& wPlayerName = va_arg(pListeElems,std::string);
            if(wPlayerName.length() != 0 )
            {
                // Handle deconnection
                throw ExceptionReseauSocketDeconnecte("Socket deconnecte");
            }
            break;
        }
    case GAME_SERVER_AUTHENTICATION_REPLY:
        {
            char* mes = va_arg(pListeElems, char*);
            mServerId = atoi(mes);
            //mServerId = *((unsigned int*)wTempServerId.c_str());
#if !SHIPPING
            std::cout << "Game server registered successfully - ID: " << mServerId << std::endl;
#endif
            break;
        }
    default:
        std::cout << "EventCode: " << pEventCode << std::endl;
        break;
    };
}

void ControllerServeurJeu::handleDisconnectDetection( SPSocket pSocket )
{
    // Doit mettre en pause la partie et notifier l'autre joueur
    std::string wSocketIdentifier = GestionnaireReseau::obtenirInstance()->getSocketIdentifier(pSocket);
    GestionnaireReseau::obtenirInstance()->removeSocket(pSocket);



    Partie* wGameBeforeCall = GameManager::obtenirInstance()->getGameWithPlayer(wSocketIdentifier);
    int wGameId = 0;
    if(wGameBeforeCall)
        wGameId = wGameBeforeCall->getUniqueGameId();
    Runnable* r = new Runnable([wGameId, wSocketIdentifier](Runnable*){
        Partie* wGame = GameManager::obtenirInstance()->getGameWithPlayer(wSocketIdentifier);
        if(wGame)
        {
            // Modifier la partie sur le serveur
            wGame->modifierEnPause(true);
            if(wGame->obtenirNomJoueurGauche() == wSocketIdentifier)
            {
                // Joueur gauche deconnecte
                wGame->modifierJoueurGauche(SPJoueurAbstrait(NULL));
            }
            else
            {
                // Joueur droit deconnecte
                wGame->modifierJoueurDroit(SPJoueurAbstrait(NULL));
            }

            // Mettre la partie en pause sur le client qui est encore connecte
            PaquetGameEvent* wPaquet = (PaquetGameEvent*) GestionnaireReseau::obtenirInstance()->creerPaquet(GAME_EVENT);
            wPaquet->setGameId(wGame->getUniqueGameId());
            wPaquet->setEvent(GAME_EVENT_PAUSE_GAME_USER_DISCONNECTED);

            std::vector<std::string> wList;
            GestionnaireReseau::obtenirInstance()->getController()->getPlayersInGame(wGame->getUniqueGameId(), wList);
            wPaquet->setNbAssociatedQueries((int) wList.size());
            if(wList.size() == 0)
            {
                // Plus personne dans la partie. On la termine (mais on ne doit pas dire au serveur maitre de sauvegarder le score etc. puisque la partie n'est pas officiellement terminee)
                // On doit mettre la partie en pause avant de la retirer de la liste pour ne pas qu'elle soit en train de tick
                GameManager::obtenirInstance()->removeGame(wGameId);
                std::cout << "Partie " << wGameId << " terminee car plus de joueurs." << std::endl;
            }
            else
            {
                for(auto it = wList.begin(); it!=wList.end(); ++it)
                {
                    if(*it != wSocketIdentifier && *it != "")
                    {
                        GestionnaireReseau::obtenirInstance()->envoyerPaquet(*it, wPaquet, TCP);
                    }
                }
            }
            
        }
    });
    RazerGameUtilities::RunOnUpdateThread(r,true);
    
}

void ControllerServeurJeu::getPlayersInGame( int pGameId, std::vector<std::string>& pPlayerList )
{
    // Code une fois que les parties vont etre sync dans les serveurs jeu
    Partie* wGame = GameManager::obtenirInstance()->getGame(pGameId);
    if(wGame)
    {
        if(wGame->obtenirNomJoueurGauche().length() > 0) {pPlayerList.push_back(wGame->obtenirNomJoueurGauche());}
        if(wGame->obtenirNomJoueurDroit().length() > 0) {pPlayerList.push_back(wGame->obtenirNomJoueurDroit());}
    }
} 


Partie* ControllerServeurJeu::getGame( int pGameId )
{
    Partie* wGame = GameManager::obtenirInstance()->getGame(pGameId);
    if(wGame)
    {

    }
    return NULL;
}
