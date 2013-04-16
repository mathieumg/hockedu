#pragma once
#include "Partie.h"
#include <queue>



class PartieApprentissage : public Partie
{
public:
    friend class GameManager;
    ~PartieApprentissage(void);

    virtual void animerBase(const float& pTime);
    virtual void animer( const float& temps );
    /*
    /// Overriden setters to make sure we only accept JoueurHumain and JoueurVirtuelRenforcement player types
    virtual void modifierJoueurDroit(SPJoueurAbstrait pPlayer);
    virtual void modifierJoueurGauche(SPJoueurAbstrait pPlayer);
    */
    /// Functions are overriden so that we can handle special behavior for learning AIs
    virtual void incrementerPointsJoueurGauche(bool pForceUpdate = false);
    virtual void incrementerPointsJoueurDroit(bool pForceUpdate = false);

    inline void clearPuckPositionBuffer() {mPuckPositions = std::queue<Vecteur3>();}

    void dumpAndConvertPlayersData();
protected:
    PartieApprentissage(GameType gameType,SPJoueurAbstrait joueurGauche = 0, SPJoueurAbstrait joueurDroit = 0, int uniqueGameId = 0, const std::vector<GameUpdateCallback>& updateCallback = std::vector<GameUpdateCallback>());

private:
    std::queue<Vecteur3> mPuckPositions;
    Vecteur3 mPreviousPuckPosition;
    bool mGoalScored;

    SPJoueurVirtuelRenforcement mLeftLearningAi;
    SPJoueurVirtuelRenforcement mRightLearningAi;

    void handleGoalScored( SPJoueurVirtuelRenforcement pLearningPlayer, SPJoueurVirtuelRenforcement pOpponent);
    void handleLearningStart( SPJoueurVirtuelRenforcement pLearningPlayer, NoeudRondelle* pPuck, NoeudMaillet* pOpponentMallet);


};

