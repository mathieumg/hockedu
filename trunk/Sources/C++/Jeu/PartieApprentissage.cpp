#include "PartieApprentissage.h"
#include "..\Arbre\Terrain.h"
#include "..\Arbre\Noeuds\NoeudRondelle.h"
#include "PlayerAbstract.h"
#include <iostream>
#include "PlayerReinforcementAI.h"
#include "..\Arbre\Noeuds\NoeudMaillet.h"
#include <memory>
#include "RazerGameTypeDef.h"
#include "GameManager.h"


int CallbackGameApprentissageStatusUpdate(int pGameId, GameStatus pGameStatus)
{
    PartieApprentissage* wGame = (PartieApprentissage*) GameManager::obtenirInstance()->getGame(pGameId);

    if(wGame && pGameStatus == GAME_ENDED)
    {
        wGame->dumpAndConvertPlayersData();
    }

    return 0;
}


PartieApprentissage::PartieApprentissage(GameType gameType,int pNbButsGagnants,SPPlayerAbstract joueurGauche, SPPlayerAbstract joueurDroit, int uniqueGameId, const std::vector<GameUpdateCallback>& updateCallback)
:Partie(gameType, pNbButsGagnants, joueurGauche, joueurDroit, uniqueGameId, updateCallback), mPreviousPuckPosition(0,0,0), mLeftLearningAi(NULL), mRightLearningAi(NULL)
{
    if(joueurGauche->obtenirType() == JOUEUR_VIRTUEL_RENFORCEMENT)
    {
        mLeftLearningAi = std::dynamic_pointer_cast<PlayerReinforcementAI>(joueurGauche);
    }
    else if(joueurDroit->obtenirType() == JOUEUR_VIRTUEL_RENFORCEMENT)
    {
        mRightLearningAi = std::dynamic_pointer_cast<PlayerReinforcementAI>(joueurDroit);
    }
    std::vector<GameUpdateCallback> wCallbacks;
    wCallbacks.push_back(CallbackGameApprentissageStatusUpdate);
    setUpdateCallback(wCallbacks);
}


PartieApprentissage::~PartieApprentissage(void)
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn PartieApprentissage::animerBase()
///
/// Overridden function in order to handle learning AI behavior on each tick.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PartieApprentissage::animerBase(const float& pTime)
{
    if( mField != NULL)
    {
        NoeudRondelle* wPuck = mField->getPuck();
        Vecteur3 wPuckPosition = wPuck->getPosition();
        NoeudMaillet* wRightMallet = mField->getRightMallet();
        NoeudMaillet* wLeftMallet = mField->getLeftMallet();
        if(mRightLearningAi && (mPreviousPuckPosition[VX] < 0 && wPuckPosition[VX] >= 0))
        {
            handleLearningStart(mRightLearningAi, wPuck, wLeftMallet);
        }
        else if(mLeftLearningAi && (mPreviousPuckPosition[VX] > 0 && wPuckPosition[VX] <= 0))
        {
            handleLearningStart(mLeftLearningAi, wPuck, wRightMallet);
        }
        mPreviousPuckPosition = wPuckPosition;

        // Code pour eviter que la puck soit stuck dans un coin
        if(getGameStatus()==GAME_STARTED)
        {
            mPuckPositions.push(wPuck->getPosition());
            if(mPuckPositions.size() > 50)
            {
                mPuckPositions.pop();
                Vecteur3 wFront = mPuckPositions.front();
                Vecteur3 wBack = mPuckPositions.back();
                Vecteur3 wDifference = wFront-wBack;
                if(wDifference.norme() < 15.0f)
                {
                    clearPuckPositionBuffer();
                    miseAuJeu(false);
                    //wGame->getField()->getPuck()->setPosition(Vecteur3());
                }
            }
        }
    }
    Partie::animerBase(pTime);

//     if(mAnimationMailletRenforcement)
//     {
//         if(mAnimationMailletRenforcement->estTermine())
//         {
//             delete mAnimationMailletRenforcement;
//             mAnimationMailletRenforcement = 0;
//         }
//         else
//         {
//             mAnimationMailletRenforcement->animer(pTime * 1000.0f); // Prends temps en ms
//         }
//     }
}

/*
////////////////////////////////////////////////////////////////////////
///
/// @fn PartieApprentissage::modifierJoueurDroit( SPJoueurAbstrait pPlayer )
///
/// Overridden function so that it limits players to either human or learning AI.
///
/// @param[in] SPJoueurAbstrait pPlayer Player to modify to
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PartieApprentissage::modifierJoueurDroit( SPJoueurAbstrait pPlayer )
{
    if(pPlayer->obtenirType() != JOUEUR_VIRTUEL)
    {
        Partie::modifierJoueurDroit(pPlayer);
    }
    else
    {
        std::cout << "Called modifierJoueurDroit with JOUEUR_VIRTUEL player type." << std::endl;
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn PartieApprentissage::modifierJoueurGauche( SPJoueurAbstrait pPlayer )
///
/// Overridden function so that it limits players to either human or learning AI.
///
/// @param[in] SPJoueurAbstrait pPlayer Player to modify to
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PartieApprentissage::modifierJoueurGauche( SPJoueurAbstrait pPlayer )
{
    if(pPlayer->obtenirType() != JOUEUR_VIRTUEL)
    {
        Partie::modifierJoueurGauche(pPlayer);
    }
    else
    {
        std::cout << "Called modifierJoueurGauche with JOUEUR_VIRTUEL player type." << std::endl;
    }
}
*/

////////////////////////////////////////////////////////////////////////
///
/// @fn PartieApprentissage::handleGoalScored( SPJoueurAbstrait pPlayer, SPJoueurAbstrait pOpponent )
///
/// Function used to handle scoring behavior for AIs.
///
/// @param[in] SPJoueurAbstrait pPlayer The player who scored.
/// @param[in] SPJoueurAbstrait pOpponent His opponent.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PartieApprentissage::handleGoalScored( SPPlayerReinforcementAI pPlayer, SPPlayerReinforcementAI pOpponent)
{
    if(pPlayer && pPlayer->isLearning())
    {
        pPlayer->setActionResult(AI_OUTPUT_BUT_COMPTE);
    }

    if(pOpponent && pOpponent->isLearning())
    {
         pOpponent->setActionResult(AI_OUTPUT_ADVERSAIRE_BUT_COMPTE);
    }
    mGoalScored = true;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn PartieApprentissage::incrementerPointsJoueurGauche( bool pForceUpdate /*= false*/ )
///
/// Overridden function so that we can handle special AI behavior when a goal is scored.
///
/// @param[in] bool pForceUpdate
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PartieApprentissage::incrementerPointsJoueurGauche( bool pForceUpdate /*= false*/ )
{
    handleGoalScored(mLeftLearningAi, mRightLearningAi);
    Partie::incrementerPointsJoueurGauche( pForceUpdate );
//#if !SHIPPING
    std::cout << "Game " << getUniqueGameId() << " - Score: " << obtenirPointsJoueurGauche() << " - " << obtenirPointsJoueurDroit() << std::endl;
//#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn PartieApprentissage::incrementerPointsJoueurGauche( bool pForceUpdate /*= false*/ )
///
/// Overridden function so that we can handle special AI behavior when a goal is scored.
///
/// @param[in] bool pForceUpdate
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PartieApprentissage::incrementerPointsJoueurDroit( bool pForceUpdate /*= false*/ )
{
    handleGoalScored(mRightLearningAi, mLeftLearningAi);
    Partie::incrementerPointsJoueurDroit( pForceUpdate );
//#if !SHIPPING
    std::cout << "Game " << getUniqueGameId() << " - Score: " << obtenirPointsJoueurGauche() << " - " << obtenirPointsJoueurDroit() << std::endl;
//#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn PartieApprentissage::handleLearningStart( NoeudMaillet* pLearningPlayer, NoeudRondelle* pPuck, NoeudMaillet* pOpponentMallet, Vecteur3 wPuckPosition )
///
/// Handles the behavior to put the AI in learning mode.
///
/// @param[in] NoeudMaillet * pLearningPlayer
/// @param[in] NoeudRondelle * pPuck
/// @param[in] NoeudMaillet * pOpponentMallet
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PartieApprentissage::handleLearningStart( SPPlayerReinforcementAI pLearningPlayer, NoeudRondelle* pPuck, NoeudMaillet* pOpponentMallet)
{
    if(pLearningPlayer->isLearning())
    {
        if(!mGoalScored)
        {
            if (pPuck->getLastHittingMallet() == pLearningPlayer->getControlingMallet())
            {
                pLearningPlayer->setActionResult(AI_OUTPUT_ADVERSAIRE_PAS_TOUCHE);
            }
            else
            {
                pLearningPlayer->setActionResult(AI_OUTPUT_RIEN);
            }
        }
        else
        {
            mGoalScored = false;
        }
		
        NoeudMaillet* wLearningMallet = pLearningPlayer->getControlingMallet();
        Vecteur3 wAiPosition(wLearningMallet->getPosition()),
                 wAiVelocity,
                 wPuckPosition(pPuck->getPosition()),
                 wPuckVelocity(pPuck->obtenirVelocite()),
                 wOpponentPosition(pOpponentMallet->getPosition());
        LearningAiAction wAction = (LearningAiAction)pLearningPlayer->getActionFor(wAiPosition, wAiVelocity, wPuckPosition, wPuckVelocity, wOpponentPosition);
        pLearningPlayer->startLearningFor(wAiPosition, wAiVelocity, wPuckPosition, wPuckVelocity, wOpponentPosition, wAction);
    }
    clearPuckPositionBuffer();
}



void PartieApprentissage::animer( const float& temps )
{
    animerBase(temps);
}

void PartieApprentissage::dumpAndConvertPlayersData()
{
    if(mRightLearningAi)
    {
        mRightLearningAi->dumpLearnedData();
        mRightLearningAi->convertLearnedData();
    }

    if(mLeftLearningAi)
    {
        mLeftLearningAi->dumpLearnedData();
        mLeftLearningAi->convertLearnedData();
    }
}

void PartieApprentissage::assignerControlesMaillet( NoeudMaillet* mailletGauche, NoeudMaillet* mailletDroit )
{
    Partie::assignerControlesMaillet(mailletGauche, mailletDroit);

    if(mRightLearningAi)
    {
        mRightLearningAi->setupFinished();
    }

    if(mLeftLearningAi)
    {
        mLeftLearningAi->setupFinished();
    }
}



bool PartieApprentissage::getReadyToPlay( bool loadMapFile /*= true */ )
{
    bool wTempValue = Partie::getReadyToPlay(loadMapFile);

    // Reset the flag for the learning players
    if(mRightLearningAi)
    {
        mRightLearningAi->setIsLearning(true);
    }
    if(mLeftLearningAi)
    {
        mLeftLearningAi->setIsLearning(true);
    }
    return wTempValue;
}
