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



int GameManager::uniqueIdCount = 0;


// Initialisations automatiques
SINGLETON_DECLARATION_CPP(GameManager);




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
    mGameAddedCallback = 0;
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
/// @param[in] bool             pForceParameters : Force l'utilisation des joueurs en parametres a la place de l'adversaire
/// @param[in] SPJoueurAbstrait pJoueur1
/// @param[in] SPJoueurAbstrait pJoueur2
///
/// @return int
///
////////////////////////////////////////////////////////////////////////
int GameManager::addNewGame(SPJoueurAbstrait pJoueur1 /*= 0*/, SPJoueurAbstrait pJoueur2 /*= 0*/, bool pForceParameters /*=false*/, int pGameId /*=-1*/)
{
	Partie* wGame;
    int wId = pGameId;
    if(pGameId = -1)
    {
        wId = GameManager::getNewUniqueGameId();
    }
	if(pJoueur2 == 0)
	{
		
		if(pForceParameters)
		{
			// Si on veut forcer la creation de la partie avec les parametres de joueurs
            wGame = new Partie(pJoueur1, pJoueur2, wId);
		}
        else if(mAdversaire)
        {
            // Si adversaire defini, on lance avec l'adversaire
            wGame = new Partie(pJoueur1, mAdversaire, wId);
        }
		else
		{
			// Si adversaire pas defini, on lance avec un joueur Humain
			wGame = new Partie(pJoueur1,SPJoueurAbstrait(new JoueurHumain("Joueur Droit")), wId);
		}
	}
	else
	{
		if(pJoueur1)
		{
			// Demarre une partie avec les 2 joueurs passes en parametre
			wGame = new Partie(pJoueur1, pJoueur2, wId);
		}
		else
		{
			// Erreur
			throw std::runtime_error("Appel invalide, le parametre 1 ne peut etre null si le deuxieme ne l'est pas");
		}
	}
    addGame(wGame);
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

	

	// Appelle le callback de gameAdded
	if(mGameAddedCallback)
	{
		mGameAddedCallback(pGame->getUniqueGameId());
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
    auto wGame = mListePartiesParId.find(pGameId);
    if(wGame != mListePartiesParId.end())
    {
        if((*wGame).second->getGameStatus() == GAME_RUNNING)
        {
            throw std::runtime_error("On ne peut appeler un remove sur une partie en cours d'execution. Doit etre terminee ou mise en pause avant.");
        }
        delete (*wGame).second;
        mListePartiesParId.erase(pGameId);
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
bool GameManager::startGame(int pGameId, const std::string& pMapFilename)
{
	Partie* wPartie = getGame(pGameId);
	wPartie->setFieldName(pMapFilename);
	if(wPartie->getReadyToPlay()) // Appelle le updateCallback avec GAME_START
    {
        wPartie->miseAuJeu(true);
        return true;
    }
    else
    {
        return false;
    }
}




Partie* GameManager::getGame( int pGameId )
{
    auto it = mListePartiesParId.find(pGameId);
    if(it!=mListePartiesParId.end())
    {
        return (*it).second;
    }
    else
    {
        return NULL;
    }
}


int GameManager::getNewUniqueGameId()
{
    return GameManager::uniqueIdCount++;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
