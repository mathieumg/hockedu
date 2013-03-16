//////////////////////////////////////////////////////////////////////////////
/// @file Partie.h
/// @author Vincent Lemire
/// @date 2012-02-17
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#pragma warning( disable : 4290 )

#include "RazerGameTypeDef.h"
#include "NoeudAffichage.h"
#include "GameTime.h"
#include "ExceptionJeu.h"
#include "PartieSyncer.h"


class NoeudMaillet;
class NoeudRondelle;

enum GameStatus {GAME_NOT_STARTED, GAME_SCORE, GAME_STARTED, GAME_ENDED, GAME_RUNNING, GAME_PAUSED};
typedef int (*GameUpdateCallback) (int, GameStatus); // Param1 = GameID, Param2 = UpdateStatus


enum PositionJoueur{GAGNANT_GAUCHE,GAGNANT_DROITE,GAGNANT_AUCUN};

///////////////////////////////////////////////////////////////////////////
/// @class Partie
/// @brief Classe représentant une partie entre 2 joueurs (virtuels ou humains).
///
/// @author Vincent Lemire
/// @date 2012-02-17
///////////////////////////////////////////////////////////////////////////
class Partie
{
public:

	friend class GameManager;
	/// Destructeur
	~Partie(void);

	/// Modificateur des points des joueurs
	void incrementerPointsJoueurGauche();
	void incrementerPointsJoueurDroit();

	/// Assignation d'un joueur à la partie, S'assurer que le pointeur est unique !!!
	void assignerJoueur( SPJoueurAbstrait joueur );

	/// Creation du noeud XML de la partie
	XmlElement* creerNoeudXML() const;
	/// Initialisation d'une partie a partir d'un noeud XML
	bool initialiserXML(const XmlElement* elem, ConteneurJoueur* profilsVirtuelsExistants = 0);

	/// Permet de reinitialiser la partie courante
	void reinitialiserPartie();

	/// Constante indiquant combien il faut de points pour gagner
	static const int POINTAGE_GAGNANT;

	/// Methode pour indiquer au maillet par qui ils sont controlles
	void assignerControlesMaillet(NoeudMaillet* mailletGauche, NoeudMaillet* mailletDroit, NoeudRondelle* rondelle) ;

	/// Permet de savoir si la partie est terminee
	inline bool partieTerminee() const {return pointsJoueurGauche_>=POINTAGE_GAGNANT || pointsJoueurDroit_>= POINTAGE_GAGNANT ;}

	/// Effectue une mise au jeu
	void miseAuJeu( bool debutDePartie = false );

	/// Cree un delais d'inactivite de (time) ms
	void delais(int time);

	/// Update la minuterie
	void updateMinuterie(int time);
	
	/// Gestion de l'affichage du décompte de mise au jeu
	void afficher();
	void animer( const float& temps);

	void vider();

	/// Termine la partie si les 2 joueur sont des AI
	bool terminerSi2AI();

	/// Retourne lobjet gameTime
	GameTime* obtenirGameTime(){return &tempsJeu_;}

    float obtenirTempsJeu(){ return tempsJeu_.Elapsed_Time_sec(); }
    void SignalGameOver();
    bool getReadyToPlay();

	inline void setUpdateCallback(GameUpdateCallback pCallback) {mUpdateCallback = pCallback;}

    inline void sendNetworkInfos() {mPartieSyncer.tick();}

/// Methode Privee
private:

    /// Constructeur par paramètres
	Partie(SPJoueurAbstrait joueurGauche = 0, SPJoueurAbstrait joueurDroit = 0, int uniqueGameId = 0, GameUpdateCallback updateCallback = 0);
    
	/// Modificateur de estPret_
	inline void modifierEstPret(bool val) { estPret_ = val; if(estPret_) tempsJeu_.unPause(); else tempsJeu_.pause(); }

    inline void setGameStatus(GameStatus pStatus) {mLastGameStatus = mGameStatus; mGameStatus = pStatus;}

/// Attributs
private:
	/// Les deux joueurs qui s'affrontent
	SPJoueurAbstrait joueurGauche_;
	SPJoueurAbstrait joueurDroit_;

	/// Les points des deux joueurs
	int pointsJoueurGauche_;
	int pointsJoueurDroit_;

	/// Indique si la partie peut debuter/continuer
	bool estPret_;
	/// Indique si la partie est en pause
	//bool enPause_;

	/// Temps restant a etre inactif
	int minuterie_;

	/// Décompte de mise au jeu
	NoeudAffichage* chiffres_;

	static GLuint listePause_;

	/// Temps ecoule depuis le debut de la partie
	GameTime tempsJeu_;

	/// Indique si cette
	bool faitPartieDunTournoi_;
    /// Terrain associé à la partie, son scope est le meme que la partie
    Terrain* mField;

	// ID unique de la partie
	int mUniqueGameId;

    // Nom associe a la partie (peut etre vide si on joue en local par exemple)
    std::string mName;
    
	// Callback pour les mise a jour de la partie
	GameUpdateCallback mUpdateCallback;

	void callGameUpdate(GameStatus pUpdateStatus) const;

    GameStatus mGameStatus;
    GameStatus mLastGameStatus;

    PartieSyncer mPartieSyncer;
    
/// Accesseurs
public:
	/// Indique si les 2 joueurs de la partie sont virtuels
	bool partieVirtuelle();
	/// Accesseur de faitPartieDunTournoi_
	bool faitPartieDunTournoi() const { return faitPartieDunTournoi_; }
	/// Modificateur de faitPartieDunTournoi_
	void modifierFaitPartieDunTournoi(bool val) { faitPartieDunTournoi_ = val; }
	/// Modificateur de enPause_
    void modifierEnPause(bool val);
	/// Accesseur de enPause_
	inline bool estEnPause() const { return mGameStatus == GAME_PAUSED; }
	/// Accesseurs des points des joueurs
	int obtenirPointsJoueurGauche() const;
	int obtenirPointsJoueurDroit() const;
	int obtenirReferencePointsJoueurGauche() {return pointsJoueurGauche_;}
	int obtenirReferencePointsJoueurDroit() {return pointsJoueurDroit_;}

	/// Accesseurs des noms des joueurs
	std::string obtenirNomJoueurGauche() const;
	std::string obtenirNomJoueurDroit() const;

	NoeudAffichage* obtenirDecompte() {return chiffres_;}

	/// Accesseur du joueur1
	inline SPJoueurAbstrait obtenirJoueurDroit() const { return joueurDroit_; }
	void modifierJoueurDroit(SPJoueurAbstrait val);

	/// Accesseur du joueur1
	inline SPJoueurAbstrait obtenirJoueurGauche() const { return joueurGauche_; }
	void modifierJoueurGauche(SPJoueurAbstrait val);

	/// Accesseur et modificateur pour le terrain
    const std::string& getFieldName() const;
    void setFieldName(const std::string& terrain);

	/// Retourne le gagnant de la partie
	SPJoueurAbstrait obtenirGagnant() const;
	/// Permet de savoir la position du gagnant
	PositionJoueur obtenirPositionGagant();
	/// Indique si la partie est prete
	bool estPret() const {return estPret_;}
    /// Accessors of mField
    inline Terrain* getField() const { return mField; }

	inline const int getUniqueGameId() {return mUniqueGameId;}

    inline GameStatus getGameStatus() const { return mGameStatus; }

    inline std::string getName() const { return mName; }
    inline void setName(std::string val) { mName = val; }

    
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


