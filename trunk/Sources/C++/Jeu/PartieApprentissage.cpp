#include "PartieApprentissage.h"
#include "..\Arbre\Terrain.h"
#include "..\Arbre\Noeuds\NoeudRondelle.h"
#include "JoueurAbstrait.h"
#include <iostream>
#include "JoueurVirtuelRenforcement.h"
#include "..\Arbre\Noeuds\NoeudMaillet.h"


PartieApprentissage::PartieApprentissage(GameType gameType,SPJoueurAbstrait joueurGauche, SPJoueurAbstrait joueurDroit, int uniqueGameId, const std::vector<GameUpdateCallback>& updateCallback)
:Partie(gameType, joueurGauche, joueurDroit, uniqueGameId, updateCallback), mPreviousPuckPosition(0,0,0)
{

}


PartieApprentissage::~PartieApprentissage(void)
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn PartieApprentissage::animer()
///
/// Overridden function in order to handle learning AI behavior on each tick.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PartieApprentissage::animer(const float& pTime)
{
    if( mField != NULL)
    {
        NoeudRondelle* wPuck = mField->getPuck();
        Vecteur3 wPuckPosition = wPuck->getPosition();
        SPJoueurAbstrait wRightPlayer = obtenirJoueurDroit();
        SPJoueurAbstrait wLeftPlayer = obtenirJoueurGauche();
        NoeudMaillet* wRightMallet = mField->getRightMallet();
        NoeudMaillet* wLeftMallet = mField->getLeftMallet();
        if(wRightPlayer->obtenirType() == JOUEUR_VIRTUEL_RENFORCEMENT && (mPreviousPuckPosition[VX] < 0 && wPuckPosition[VX] >= 0))
        {
            handleLearningStart(wRightMallet, wPuck, wLeftMallet);
        }
        else if(wLeftPlayer->obtenirType() == JOUEUR_VIRTUEL_RENFORCEMENT && (mPreviousPuckPosition[VX] > 0 && wPuckPosition[VX] <= 0))
        {
            handleLearningStart(wLeftMallet, wPuck, wRightMallet);
        }
        mPreviousPuckPosition = wPuckPosition;
    }
    Partie::animerBase(pTime);
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
void PartieApprentissage::handleGoalScored( SPJoueurAbstrait pPlayer, SPJoueurAbstrait pOpponent)
{
    if(pPlayer->obtenirType() == JOUEUR_VIRTUEL_RENFORCEMENT)
    {
        //TODO Make AILearner take a player object to save data
        //((JoueurVirtuelRenforcement*)pPlayer.get())->
        AILearner::obtenirInstance()->terminerSauvegardeNouvelleInfo(AI_OUTPUT_ADVERSAIRE_BUT_COMPTE);
    }

    if(pOpponent->obtenirType() == JOUEUR_VIRTUEL_RENFORCEMENT)
    {
        AILearner::obtenirInstance()->terminerSauvegardeNouvelleInfo(AI_OUTPUT_BUT_COMPTE);
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
    handleGoalScored(obtenirJoueurGauche(), obtenirJoueurDroit());
    Partie::incrementerPointsJoueurGauche( pForceUpdate );
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
    handleGoalScored(obtenirJoueurDroit(), obtenirJoueurGauche());
    Partie::incrementerPointsJoueurGauche( pForceUpdate );
}

////////////////////////////////////////////////////////////////////////
///
/// @fn PartieApprentissage::handleLearningStart( NoeudMaillet* pLearningMallet, NoeudRondelle* pPuck, NoeudMaillet* pOpponentMallet, Vecteur3 wPuckPosition )
///
/// Handles the behavior to put the AI in learning mode.
///
/// @param[in] NoeudMaillet * pLearningMallet
/// @param[in] NoeudRondelle * pPuck
/// @param[in] NoeudMaillet * pOpponentMallet
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PartieApprentissage::handleLearningStart( NoeudMaillet* pLearningMallet, NoeudRondelle* pPuck, NoeudMaillet* pOpponentMallet)
{
    if(!mGoalScored)
    {
        AILearner::obtenirInstance()->terminerSauvegardeNouvelleInfo(AI_OUTPUT_RIEN);
    }
    else
    {
        mGoalScored = false;
    }
    Vecteur3 wAiPosition(pLearningMallet->getPosition()),
        wAiVelocity(pLearningMallet->obtenirVelocite()),
        wPuckPosition(pPuck->getPosition()),
        wPuckVelocity(pPuck->obtenirVelocite()),
        wOpponentPosition(pOpponentMallet->getPosition());
    LearningAiAction wAction = (LearningAiAction) (rand() % AI_ACTION_NB);
    AILearner::obtenirInstance()->sauvegarderNouvelleInfo(wAiPosition, wAiVelocity, wPuckPosition, wPuckVelocity, wOpponentPosition, wAction);
}
