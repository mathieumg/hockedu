#pragma once
#include "Partie.h"



class PartieApprentissage : public Partie
{
public:
    ~PartieApprentissage(void);

    virtual void animer(const float& pTime);

    /// Overriden setters to make sure we only accept JoueurHumain and JoueurVirtuelRenforcement player types
    virtual void modifierJoueurDroit(SPJoueurAbstrait pPlayer);
    virtual void modifierJoueurGauche(SPJoueurAbstrait pPlayer);

    /// Functions are overriden so that we can handle special behavior for learning AIs
    virtual void incrementerPointsJoueurGauche(bool pForceUpdate = false);
    virtual void incrementerPointsJoueurDroit(bool pForceUpdate = false);
protected:
    PartieApprentissage(GameType gameType,SPJoueurAbstrait joueurGauche = 0, SPJoueurAbstrait joueurDroit = 0, int uniqueGameId = 0, const std::vector<GameUpdateCallback>& updateCallback = std::vector<GameUpdateCallback>());

private:
    Vecteur3 mPreviousPuckPosition;
    bool mGoalScored;

    void handleGoalScored( SPJoueurAbstrait pLearningPlayer, SPJoueurAbstrait pOpponent);
};

