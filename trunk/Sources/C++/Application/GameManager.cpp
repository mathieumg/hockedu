///////////////////////////////////////////////////////////////////////////////
/// @file GameManager.cpp
/// @author Mathieu Parent
/// @date 2013-03-03
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "GameManager.h"
#include <sstream>
#include <stdexcept>
#include "JoueurVirtuel.h"
#include "JoueurHumain.h"
#include "..\Reseau\Paquets\PaquetGameEvent.h"
#include "..\Reseau\RelayeurMessage.h"
#include "..\Reseau\GestionnaireReseau.h"
#include "GestionnaireHUD.h"
#include "Runnable.h"
#include "..\Reseau\Paquets\PaquetGameStatus.h"
#include "..\Reseau\ObjetsGlobaux\PartieServeurs.h"
#include "PartieApprentissage.h"



int GameManager::uniqueIdCount = 0;

HANDLE_MUTEX GameManager::mMutexTickRemove;


// Initialisations automatiques
SINGLETON_DECLARATION_CPP(GameManager);


// Callback pour sync avec le serveur Maitre
int CallbackGameUpdateServeurJeu(int pGameId, GameStatus pGameStatus)
{
    Partie* wGame = GameManager::obtenirInstance()->getGame(pGameId);

    if(wGame && wGame->isNetworkServerGame())
    {
        // On envoie les infos au serveur maitre
        PaquetGameStatus* wPaquet = (PaquetGameStatus*) UsinePaquet::creerPaquet(GAME_STATUS);
        PartieServeurs* wPartie = wGame->buildPartieServeurs();
        wPaquet->setGameInfos(wPartie); // S'occupe du delete
        GestionnaireReseau::obtenirInstance()->envoyerPaquet("MasterServer", wPaquet, TCP);

    }
    else if(wGame && wGame->isNetworkClientGame() && pGameStatus == GAME_ENDED)
    {
        // On signal au serveur jeu qu'on quitte la partie
        PaquetGameEvent* wPaquet = (PaquetGameEvent*) UsinePaquet::creerPaquet(GAME_EVENT);
        wPaquet->setGameId(wGame->getUniqueGameId());
        wPaquet->setEvent(GAME_EVENT_PAUSE_GAME_USER_DISCONNECTED);
        wPaquet->setEventOnPlayerLeft(wGame->obtenirNomJoueurGauche() == GestionnaireReseau::obtenirInstance()->getPlayerName());
        GestionnaireReseau::obtenirInstance()->envoyerPaquet("GameServer", wPaquet, TCP);
    }
    return 0;
}


///////////////////////////////////////////////////////////////////
///
/// @fn  GameManager::GameManager(  )
///
/// Constructeur
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GameManager::GameManager()
{
	mMaximumGameCount = 100000; // Simule pas de limite
	mAdversaire = 0;
    // Ne pas mettre a 0, trop dangereux
    GameManager::uniqueIdCount = 1;// = rand() % INT_MAX/2;
    FacadePortability::createMutex(GameManager::mMutexTickRemove);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn  GameManager::~GameManager(  )
///
/// Destructeur Vide
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GameManager::~GameManager()
{
    // On vide le contenu des maps
    for(auto it = mListePartiesParId.begin(); it != mListePartiesParId.end(); ++it)
    {
        delete (*it).second;
    }

    mListePartiesParId.clear();
}



////////////////////////////////////////////////////////////////////////
///
/// @fn int GameManager::addNewGame( SPJoueurAbstrait pJoueur1 /*= 0*/, SPJoueurAbstrait pJoueur2 /*= 0*/ )
///
/// Ajoute une nouvelle partie contre l'adversaire precedemment ajuste, 
/// Si les 2 parametres sont nulles, une partie AI contre AI sera cree
/// Si un seul parametre est utilise, une partie contre l'adversaire sera cree
/// Dans le cas ou l'adversaire n'a pas ete modifie, on joue contre un joueur humain
///
/// @param[in] int              pNbButsGagnants  : Nb de buts gagnants dans la partie
/// @param[in] bool             pForceParameters : Force l'utilisation des joueurs en parametres a la place de l'adversaire
/// @param[in] SPJoueurAbstrait pJoueur1
/// @param[in] SPJoueurAbstrait pJoueur2
///
/// @return int
///
////////////////////////////////////////////////////////////////////////
int GameManager::addNewGame(GameType gametype, int pNbButsGagnants, SPJoueurAbstrait pJoueur1 /*= 0*/, SPJoueurAbstrait pJoueur2 /*= 0*/, bool pForceParameters /*=false*/,  bool pIsLearningGame /*=false*/, int pGameId /*=-1*/)
{
	Partie* wGame;
    int wId = pGameId;
    SPJoueurAbstrait wJoueur1, wJoueur2;
    GameType wGameType = gametype;
    if(pGameId == -1)
    {
        wId = GameManager::getNewUniqueGameId();
    }
	if(pJoueur2 == 0)
	{
        // Joueur 1 toujours setté
        wJoueur1 = pJoueur1;
        if(pForceParameters)
        {
            // Si on veut forcer la creation de la partie avec les parametres de joueurs
            wJoueur2 = pJoueur2;
		}
        else if(mAdversaire)
        {
            // Si adversaire defini, on lance avec l'adversaire
            wJoueur2 = mAdversaire;
        }
		else
		{
            // Si adversaire pas defini, on lance avec un joueur Humain
            wJoueur2 = SPJoueurAbstrait(new JoueurHumain("Joueur Droit"));
		}
	}
	else
	{
		if(pJoueur1)
		{
            wJoueur1 = pJoueur1;
            wJoueur2 = pJoueur2;
		}
		else
		{
			// Erreur
			checkf(0)
		}
	}
    if(pIsLearningGame)
    {
        wGame = new PartieApprentissage(wGameType, pNbButsGagnants, wJoueur1, wJoueur2, wId);
    }
    else
    {
        wGame = new Partie(wGameType, pNbButsGagnants, wJoueur1, wJoueur2, wId);
    }

    bool wGameAdded = false;
    int wTryCount = 0;
    std::vector<GameUpdateCallback> wListeCallbacks;
    wListeCallbacks.push_back(CallbackGameUpdateServeurJeu);
    wGame->setUpdateCallback(wListeCallbacks);
    while(!wGameAdded && wTryCount <1000)
    {
        try
        {
            addGame(wGame);
            wGameAdded = true;
        }
        catch(std::runtime_error&)
        {
            // Une partie existe probablement avec cet id et on peut pas sauvegarder la nouvelle, on lui donne un nouvel id
            wGame->setUniqueGameId(getNewUniqueGameId());
            ++wTryCount;
        }
    }
    // Si tu pogne ce checkf, t'as un gros probleme
    checkf(wTryCount < 1000);

	return wGame->getUniqueGameId();
}





////////////////////////////////////////////////////////////////////////
///
/// @fn void GameManager::addGame( Partie* pPartie )
///
/// Ajout d'une partie deja creee
///
/// @param[in] Partie * pPartie
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameManager::addGame( Partie* pGame )
{
	// Si on ne depasse pas la taille maximale fixee
	if(mListePartiesParId.size() < mMaximumGameCount)
	{
		// Ajout dans la hashmap
		if(mListePartiesParId.find(pGame->getUniqueGameId()) != mListePartiesParId.end())
		{
			throw std::runtime_error("Une autre partie avec cet id est deja contenue dans le GameManager");
		}
        mListePartiesParId[pGame->getUniqueGameId()] = pGame;
	}
	else
	{
		std::stringstream ss;
		ss << "Le GameManager ne peut contenir plus de " << mMaximumGameCount << "parties en meme temps.";
		throw std::runtime_error(ss.str());
	}

	

	// Appelle les callbacks de gameAdded
    for(auto it = mGameAddedCallbacks.begin(); it!=mGameAddedCallbacks.end(); ++it)
    {
        (*it)(pGame->getUniqueGameId());
    }
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void GameManager::removeGame( int pGameId )
///
/// Methode pour retirer une partie terminee
///
/// @param[in] int pGameId : Id de la partie a supprimer
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameManager::removeGame( int pGameId )
{
    int i=0;
    Runnable* r = new Runnable([pGameId](Runnable*){
        FacadePortability::takeMutex(mMutexTickRemove);
        auto wGame = GameManager::obtenirInstance()->getGame(pGameId);
        if(wGame)
        {
            if(wGame->getGameStatus() != GAME_PAUSED)
            {
                wGame->modifierEnPause(true);
            }
            wGame->SignalGameOver();
            delete wGame;
            GameManager::obtenirInstance()->removeGameFromList(pGameId);
        }
        FacadePortability::releaseMutex(mMutexTickRemove);
    });
    RazerGameUtilities::RunOnUpdateThread(r,true);
}



////////////////////////////////////////////////////////////////////////
///
/// @fn bool GameManager::startGame()
///
/// Methode pour demarrer une partie
///
/// @param[in] int          pGameId:        Id de la partie a traiter
/// @param[in] std::string  pMapFilename:   Nom de la map a utiliser
///
/// @return bool : Succes ou fail (pas utilise pour le moment)
///
////////////////////////////////////////////////////////////////////////
bool GameManager::getGameReady(int pGameId)
{
    Partie* wPartie = getGame(pGameId);
    
    if(!wPartie)
    {
        return false;
    }


    if(wPartie->getReadyToPlay()) // Appelle le updateCallback avec GAME_START
    {

        if(wPartie->isNetworkClientGame())
        {
            // Dans ce cas avant de demarrer la partie, on envoie un message au serveur jeu pour dire que le client est pret et il va nous dire quand demarrer
            PaquetGameEvent* wPaquet = (PaquetGameEvent*) UsinePaquet::creerPaquet(GAME_EVENT);
            wPaquet->setGameId(pGameId);
            wPaquet->setEvent(GAME_EVENT_READY);
            wPaquet->setEventOnPlayerLeft(GestionnaireReseau::obtenirInstance()->getPlayerName() == wPartie->obtenirNomJoueurGauche());
            RelayeurMessage::obtenirInstance()->relayerPaquetGame(pGameId, wPaquet, TCP);
        }
        wPartie->setGameStatus(GAME_READY);
        return true;
    }
    else
    {
        return false;
    }

}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool GameManager::startGame()
///
/// Methode pour demarrer une partie
///
///
/// @return bool : Succes ou fail (pas utilise pour le moment)
///
////////////////////////////////////////////////////////////////////////
bool GameManager::startGame(int pGameId)
{
	Partie* wPartie = getGame(pGameId);

    if(!wPartie)
    {
        return false;
    }

    if(wPartie->getGameStatus() == GAME_NOT_READY)
    {
        // On initialise la partie

        if(!getGameReady(pGameId))
        {
            return false;
        }
    }
    
    if(wPartie->getGameStatus() == GAME_READY)
    {
        if(GestionnaireHUD::Exists())
        {
            GestionnaireHUD::obtenirInstance()->setForeverAloneVisibility(false);
        }
        // Start game si partie deja ready
        wPartie->miseAuJeu(true);
    }
    
    return true;
    

}




Partie* GameManager::getGame( int pGameId )
{
    auto it = mListePartiesParId.find(pGameId);
    if(it!=mListePartiesParId.end())
    {
        return it->second;
    }
    else
    {
        return NULL;
    }
}



Partie* GameManager::getGame( const std::string& pGameName )
{
    auto it = mListePartiesParId.begin();
    for(it; it!=mListePartiesParId.end(); ++it)
    {
        Partie* wPartie = (*it).second;
        if(wPartie)
        {
            if(pGameName == wPartie->getName()) {
                return wPartie;
            }
        }
    }
    return NULL;
}


int GameManager::getNewUniqueGameId()
{
    return GameManager::uniqueIdCount++;
}




void GameManager::animer( const float& pTemps )
{
    // On anime toutes les parties
    for(auto it = mListePartiesParId.begin(); it!=mListePartiesParId.end(); ++it)
    {
        if((*it).second->getGameStatus() >= GAME_READY)
        {
            // Anime si la partie est en cours (n'animera pas si la partie n'est pas encore commencee)
            (*it).second->animerBase(pTemps);
        }
    }
}



bool GameManager::setMapForGame( int pGameId, const std::string& pMapName )
{
    Partie* wGame = getGame(pGameId);

    if(wGame)
    {
        wGame->setFieldName(pMapName);
        return true;
    }


    return false;
}



Partie* GameManager::getGameWithPlayer( const std::string& pPlayerName )
{
    for(auto it = mListePartiesParId.begin(); it!=mListePartiesParId.end(); ++it)
    {
        Partie* wGame = it->second;
        if(wGame)
        {
            if(wGame->obtenirNomJoueurGauche() == pPlayerName || wGame->obtenirNomJoueurDroit() == pPlayerName)
            {
                return wGame;
            }
        }
    }
    return NULL;
}








///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
