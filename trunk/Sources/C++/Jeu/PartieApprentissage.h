#pragma once
#include "Partie.h"
#include <queue>
#include "Animation.h"



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

    inline void setAnimationMailletRenforcement(Animation* val)
    {
        if(mAnimationMailletRenforcement)
        {
            delete mAnimationMailletRenforcement;
        }
        mAnimationMailletRenforcement = val;
    }

    /// Overriden so we can setup the AILearner output file once mallets are assigned.
    virtual void assignerControlesMaillet(NoeudMaillet* mailletGauche, NoeudMaillet* mailletDroit, NoeudRondelle* rondelle);
protected:
    PartieApprentissage(GameType gameType, int pNbButsGagnants, SPPlayerAbstract joueurGauche = 0, SPPlayerAbstract joueurDroit = 0, int uniqueGameId = 0, const std::vector<GameUpdateCallback>& updateCallback = std::vector<GameUpdateCallback>());

private:
    std::queue<Vecteur3> mPuckPositions;
    Vecteur3 mPreviousPuckPosition;
    bool mGoalScored;

    SPPlayerReinforcementAI mLeftLearningAi;
    SPPlayerReinforcementAI mRightLearningAi;

    void handleGoalScored( SPPlayerReinforcementAI pLearningPlayer, SPPlayerReinforcementAI pOpponent);
    void handleLearningStart( SPPlayerReinforcementAI pLearningPlayer, NoeudRondelle* pPuck, NoeudMaillet* pOpponentMallet);

    virtual bool getReadyToPlay( bool loadMapFile = true );

    Animation* mAnimationMailletRenforcement;
    
};

