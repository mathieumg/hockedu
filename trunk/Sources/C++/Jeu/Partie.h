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

#if WIN32
#include "PartieSyncer.h"
#include "..\Reseau\ObjetsGlobaux\PartieServeurs.h"
#endif

#include "RazerGameTypeDef.h"
#include "NoeudAffichage.h"
#include "GameTime.h"
#include "ExceptionJeu.h"
#include "ReplayObserver.h"
#include "AchievementsEnums.h"
#include <vector>


class NoeudMaillet;
class NoeudRondelle;
class AIRenforcementTest;
typedef int (*GameUpdateCallback) (int, GameStatus); // Param1 = GameID, Param2 = UpdateStatus


enum PositionJoueur{GAGNANT_GAUCHE,GAGNANT_DROITE,GAGNANT_AUCUN};
/// Permet de connaitre l'intention de la partie et qui la contient
enum GameType
{
    GAME_TYPE_NETWORK_CLIENT,   /// Some event will not be simulated since a server is required to play the game
    GAME_TYPE_NETWORK_SERVER,   /// will send some information on the network to keep client games in sync
    GAME_TYPE_OFFLINE,          /// Must do all the simulation and event locally
};


struct PuckProjection 
{
    int time;
    Vecteur2 position;
    Vecteur2 velocite;
};

///////////////////////////////////////////////////////////////////////////
/// @class Partie
/// @brief Classe repr�sentant une partie entre 2 joueurs (virtuels ou humains).
///
/// @author Vincent Lemire
/// @date 2012-02-17
///////////////////////////////////////////////////////////////////////////
class Partie : public ReplayObserver
{
public:
    friend class SourisEtatPIEMode;
	friend class GameManager;

	friend AIRenforcementTest;
	/// Destructeur
	~Partie(void);

	/// Modificateur des points des joueurs
	virtual void incrementerPointsJoueurGauche(bool pForceUpdate = false);
	virtual void incrementerPointsJoueurDroit(bool pForceUpdate = false);

	/// Assignation d'un joueur � la partie, S'assurer que le pointeur est unique !!!
	void assignerJoueur( SPPlayerAbstract joueur );

	/// Creation du noeud XML de la partie
	XmlElement* creerNoeudXML() const;
	/// Initialisation d'une partie a partir d'un noeud XML
	bool initialiserXML(const XmlElement* elem, ConteneurJoueur* profilsVirtuelsExistants = 0);

	/// Permet de reinitialiser la partie courante
	void reinitialiserPartie();

	/// Constante indiquant combien il faut de points pour gagner
	static const int POINTAGE_GAGNANT;

	/// Methode pour indiquer au maillet par qui ils sont controlles
	virtual void assignerControlesMaillet(NoeudMaillet* mailletGauche, NoeudMaillet* mailletDroit);

    void reloadControleMallet();
    
	/// Permet de savoir si la partie est terminee
	inline bool partieTerminee() const {return pointsJoueurGauche_>=mNbButsGagnants || pointsJoueurDroit_>= mNbButsGagnants ;}

	/// Effectue une mise au jeu
	void miseAuJeu( bool debutDePartie = false);

	/// Cree un delais d'inactivite de (time) ms
	void delais(int time);

	/// Update la minuterie
	void updateMinuterie(int time);
	
	/// Gestion de l'affichage du d�compte de mise au jeu
	void afficher();
	virtual void animer( float temps);
    virtual void animerBase( float temps );

	void vider();

	/// Termine la partie si les 2 joueur sont des AI
	bool terminerSi2AI();

	/// Retourne lobjet gameTime
	GameTime* obtenirGameTime(){return &tempsJeu_;}

    float obtenirTempsJeu(){ return tempsJeu_.Elapsed_Time_sec(); }
    // Temps en sec
    void setTempsJeu(float pElapsedTime) {tempsJeu_.adjustTime(pElapsedTime * 1000.0f);}

    void SignalGameOver();
    virtual bool getReadyToPlay(bool loadMapFile = true);

	inline void setUpdateCallback(const std::vector<GameUpdateCallback>& pCallbacks) {mUpdateCallbacks.insert(mUpdateCallbacks.end(), pCallbacks.begin(), pCallbacks.end());}

    inline void sendNetworkInfos() {mPartieSyncer.tick();}

    bool validatePassword(const std::string& pPasswordToValidate) const;
    void setPassword(const std::string& pPassword);
    inline bool requirePassword() const {return mRequirePassword;}

    inline bool isNetworkClientGame() const
    {
        return mGameType == GAME_TYPE_NETWORK_CLIENT;
    }
    inline bool isNetworkServerGame() const
    {
        return mGameType == GAME_TYPE_NETWORK_SERVER;
    }
    inline bool isOfflineGame() const
    {
        return mGameType == GAME_TYPE_OFFLINE;
    }

    virtual void updateObserver( const ReplaySubject* pSubject );

    void SendAchievementEventToHumanPlayer(SPPlayerAbstract player,AchievementEvent eventIfHuman, AchievementEvent eventIfNonHuman );

    void setGameStatus(GameStatus pStatus);
    
    PartieServeurs* buildPartieServeurs();  

    // Met a jour les informations contenues dans le terrain de simulation selon ce qui est contenu dans le terrain de base
    bool updateTerrainSimulation();

    // Retourne la projection de quand la position de la puck est a ce X
    // Retourne la position et dans combien de temps
    // Si le temps == -1, delais max depasse
    PuckProjection getPuckProjection(float pPosX, int pDelaisMaxMs = 1000); 


    /// Accessors of mMiseAuJeuDelai
    inline int getMiseAuJeuDelai() const { return mMiseAuJeuDelai; }
    inline void setMiseAuJeuDelai(const int& pVal) { mMiseAuJeuDelai = pVal; }

    inline float getTempsPlus() const { return mTempsPlus; }
    inline void setTempsPlus(float val) { mTempsPlus = val; }

/// Protected because we need to call these from the class' children
protected:

    /// Constructeur par param�tres
	Partie(GameType gameType, int pNbButsGagnants,SPPlayerAbstract joueurGauche = 0, SPPlayerAbstract joueurDroit = 0, int uniqueGameId = 0, const std::vector<GameUpdateCallback>& updateCallback = std::vector<GameUpdateCallback>());

    /// Terrain associ� � la partie, son scope est le meme que la partie
    Terrain* mField;

    /// Les deux joueurs qui s'affrontent
	SPPlayerAbstract joueurGauche_;
	SPPlayerAbstract joueurDroit_;

/// Attributs
private:
	
    int mMiseAuJeuDelai;

    int mNbButsGagnants;

	/// Les points des deux joueurs
	int pointsJoueurGauche_;
	int pointsJoueurDroit_;

	/// Temps restant a etre inactif
	clock_t mClockDelaisDone;
    clock_t mClockLastTick;

	/// D�compte de mise au jeu
	NoeudAffichage* chiffres_;

	static GLuint listePause_;

	/// Temps ecoule depuis le debut de la partie
	GameTime tempsJeu_;
    float mTempsPlus; // Temps de depart en sec
    
	/// Indique si cette
	bool faitPartieDunTournoi_;

    /// Terrain pour les simulations physiques
    Terrain* mFieldSimulation;

	// ID unique de la partie
	int mUniqueGameId;

    // Nom associe a la partie (peut etre vide si on joue en local par exemple)
    std::string mName;
    
	// Callback pour les mise a jour de la partie
	std::vector<GameUpdateCallback> mUpdateCallbacks;

	void callGameUpdate(GameStatus pUpdateStatus) const;

    GameStatus mGameStatus;
    GameStatus mLastGameStatus;

    PartieSyncer mPartieSyncer;
	
    bool mRequirePassword;
    std::string mPassword;
    
    /// Permet de connaitre l'intention de la partie et qui la contient
    /// ne peux changer une fois cr�er
    const GameType mGameType;


    
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

	/// Accesseurs des noms des joueurs
	std::string obtenirNomJoueurGauche() const;
	std::string obtenirNomJoueurDroit() const;

	NoeudAffichage* obtenirDecompte() {return chiffres_;}

	/// Accesseur du joueur1
	inline SPPlayerAbstract obtenirJoueurDroit() const { return joueurDroit_; }
	virtual void modifierJoueurDroit(SPPlayerAbstract val);

	/// Accesseur du joueur1
	inline SPPlayerAbstract obtenirJoueurGauche() const { return joueurGauche_; }
	virtual void modifierJoueurGauche(SPPlayerAbstract val);

	/// Accesseur et modificateur pour le terrain
    const std::string& getFieldName() const;
    void setFieldName(const std::string& terrain);

	/// Retourne le gagnant de la partie
	SPPlayerAbstract obtenirGagnant() const;
	/// Permet de savoir la position du gagnant
	PositionJoueur obtenirPositionGagant();
    /// Accessors of mField
    inline Terrain* getField() const { return mField; }

    inline const int getUniqueGameId() {return mUniqueGameId;}
	inline void setUniqueGameId(int pNewGameId) {mUniqueGameId = pNewGameId;}

    inline GameStatus getGameStatus() const { return mGameStatus; }

    inline std::string getName() const { return mName; }
    inline void setName(std::string val) { mName = val; }

    inline PartieSyncer* getPartieSyncer() { return &mPartieSyncer; }

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


