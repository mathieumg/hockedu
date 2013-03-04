/*///////////////////////////////////////////////////////////////////////////////
/// @file GameManager.cpp
/// @author Mathieu Parent
/// @date 2013-03-03
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "GameManager.h"


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

    // Pas de delete dans le 2e car deja delete
    mListePartiesParPlayerName.clear();
    mListePartiesParId.clear();
}



void GameManager::addNewGame( PartieServeurs* pGame )
{
    mListePartiesParId[pGame->getUniqueGameId()] = pGame;
    mListePartiesParPlayerName[std::pair<std::string, std::string>(pGame->getPlayer1Name(), pGame->getPlayer2Name())] = pGame;

}



void GameManager::updateGameStatus( PartieServeurs* pGame )
{
    //asdasda
}



int GameManager::startNewGame()
{
    PartieServeurs* wGame = new PartieServeurs;
    addNewGame(wGame);
    mGameStartedCallback(wGame->getUniqueGameId());

}





///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

*/