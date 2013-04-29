//////////////////////////////////////////////////////////////////////////////
/// @file JoueurVirtuelRenforcement.h
/// @author Mathieu Parent
/// @date 2013-04-1
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "PlayerComputer.h"
#include <map>
#include "Renforcement\AILearner.h"
#include "BoiteEnvironnement.h"
#include "Utilitaire.h"


///////////////////////////////////////////////////////////////////////////
/// @class JoueurVirtuelRenforcement
/// @brief Classe qui représente un joueur contrôlé par l'ordinateur selon l'apprentissage par renforcement.
///
/// @author Mathieu Parent
/// @date 2013-04-1
///////////////////////////////////////////////////////////////////////////
class PlayerReinforcementAI : public PlayerComputer
{
public:
    friend class AiLearnerTests;
	/// Constructeur par paramètres
	PlayerReinforcementAI(const std::string& nom = "", unsigned int vitesse = 0, unsigned int probabiliteEchec = 0, bool isLearning = true);


	/// Destructeur virtuel
	virtual ~PlayerReinforcementAI(void);

	/// Creation du noeud XML du joueur
	virtual XmlElement* creerNoeudXML() const;

    /// Permet d'obtenir l'action a utiliser pour une certaine situation donnee
    AiRuntimeInfosOutput getActionFor(const Vecteur3& pPositionAi, const Vecteur3& pVelociteAi, const Vecteur3& pPositionRondelle, const Vecteur3& pVelociteRondelle, const Vecteur3& pPositionJoueurAdverse);

	/// Questionne le AIMaillet sur la direction à prendre du maillet
	virtual Vecteur2 obtenirDirectionAI(NoeudMaillet* maillet);

    inline std::string GetAiLogicFilepath() const {return mAiLogicFilepath;}

    virtual void setControlingMallet(class NoeudMaillet* pVal);
    virtual void setPlayerSide(PlayerSide val);

    void dumpLearnedData() { mAiLearner.dump(); }

    void startLearningFor(const Vecteur3& pPositionAi, const Vecteur3& pVelociteAi, const Vecteur3& pPositionRondelle, const Vecteur3& pVelociteRondelle, const Vecteur3& pPositionJoueurAdverse, LearningAiAction pAction)
    {
        mAiLearner.sauvegarderNouvelleInfo(pPositionAi, pVelociteAi, pPositionRondelle, pVelociteRondelle, pPositionJoueurAdverse, pAction);
    }

    void setActionResult(LearningAiOutput pResult) { mAiLearner.terminerSauvegardeNouvelleInfo(pResult); }

    bool isLearning() const { return mIsLearning; }
    void setIsLearning(const bool& pIsLearning) { mIsLearning = pIsLearning; }

    void convertLearnedData() { mAiLearner.convertirDonneesRaw([&](bool pSuccess)->int { utilitaire::afficherErreur("Conversion terminee"); mIsConversionDone = true; return 0;  }); }

    bool isConversionDone() const { return mIsConversionDone; }

    void setupFinished();

    virtual void modifierNom(const std::string nom);

private:
    bool hasMapEntryFor(const Vecteur3& pPositionAi, const Vecteur3& pVelociteAi, const Vecteur3& pPositionRondelle, const Vecteur3& pVelociteRondelle, const Vecteur3& pPositionJoueurAdverse) const;

    bool chargerAiLogic(const std::string& pAiLogicFilepath);

    AiRuntimeInfosInput convertInputData( const Vecteur3 &pPositionAi, const Vecteur3 &pPositionJoueurAdverse, const Vecteur3 &pPositionRondelle, const Vecteur3 &pVelociteAi, const Vecteur3 &pVelociteRondelle ) const;

    bool mIsConversionDone;

    std::map<AiRuntimeInfosInput, AiRuntimeInfosOutput> mActionMap;

    std::string mAiLogicFilepath;

    AILearner mAiLearner;
    bool mIsLearning;

    unsigned int mLearningCounter;
protected:
	/// Initialisaiton du joueur à partir d'un element XML
	virtual bool initialiser(const XmlElement* element);
    
    virtual void gameInit();

	virtual void PlayTick( float time );

};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
