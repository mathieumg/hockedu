/*//////////////////////////////////////////////////////////////////////////////
/// @file GameManager.h
/// @author Mathieu Parent
/// @date 2013-03-03
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <map>
#include <string>
#include "Singleton.h"
#include <hash_map>
#include "..\Reseau\ObjetsGlobaux\PartieServeurs.h"


typedef int (*GameManagerCallback) (int); // Param = GameID

///////////////////////////////////////////////////////////////////////////
/// @class GameManager
/// @brief Classe qui permet degerer plusieurs parties
///
/// @author Mathieu Parent
/// @date 2013-03-03
///////////////////////////////////////////////////////////////////////////
class GameManager:public Singleton<GameManager>
{
	SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(GameManager);
public:
	/// Constructeur par defaut
	GameManager();
	/// Destructeur
	~GameManager();

    inline void setMaximumGameCount(int pGameCount) {mMaximumGameCount = pGameCount;}

    void addNewGame(PartieServeurs* pGame);
    void updateGameStatus(PartieServeurs* pGame);

    int startNewGame();

    inline void setGameStartedCallback(GameManagerCallback pCallback) {mGameStartedCallback = pCallback;}
    inline void setGameUpdatedCallback(GameManagerCallback pCallback) {mGameUpdatedCallback = pCallback;}
private:
    int mMaximumGameCount;

    // Les 2 hashmap contiennent les memes infos, mais cela rend la recherche beaucoup plus efficace
    std::hash_map<int, PartieServeurs*> mListePartiesParId;
    std::hash_map<std::pair<std::string, std::string>, PartieServeurs*> mListePartiesParPlayerName;

    GameManagerCallback mGameStartedCallback;
    GameManagerCallback mGameUpdatedCallback;

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




*/