//////////////////////////////////////////////////////////////////////////////
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
#include "../Jeu/Partie.h"


typedef int (*GameAddedCallback) (int); // Param1 = GameID

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

	void removeGame(int pGameId);

	bool startGame(int pGameId, const std::string& pMapFilename);

	Partie* getGame(int pGameId);


	// Ajoute une nouvelle partie contre l'adversaire precedemment ajuste, 
	// Si les 2 parametres sont nulles, une partie AI contre AI sera cree
	// Si un seul parametre est utilise, une partie contre l'adversaire sera cree
	// Dans le cas ou l'adversaire n'a pas ete modifie, on joue contre un joueur humain
	int addNewGame(SPJoueurAbstrait pJoueur1 = 0, SPJoueurAbstrait pJoueur2 = 0, bool pForceParameters = false, int pGameId = -1);


	// Methodes pour sauvegarder des fonctions de callback qui seront appelees lors d'ajout de parties ou lors de la modification des parties
    inline void setGameAddedCallback(GameAddedCallback pCallback) {mGameAddedCallback = pCallback;}
	// ATTENTION, le updateCallback utilise pour une partie depend uniquement de celui defini a la creation de la partie. Une modification n'a pas d'impact sur les parties deja crees
    inline void setGameUpdateCallback(GameUpdateCallback pCallback) {mGameUpdatedCallback = pCallback;}

	inline void setAdversaire(SPJoueurAbstrait val) {mAdversaire = val;}
private:
    int mMaximumGameCount;

    std::hash_map<int, Partie*> mListePartiesParId;

    GameAddedCallback mGameAddedCallback;
    GameUpdateCallback mGameUpdatedCallback;


	// Ajout d'une partie deja creee
	void addGame(Partie* pGame);

	// Adversaire a utiliser pour la creation de partie (ATTENTION, une fois la partie lancee, l'adversaire est reinitialise)
	SPJoueurAbstrait mAdversaire;
	
    static int uniqueIdCount;

    static int getNewUniqueGameId();
	
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




