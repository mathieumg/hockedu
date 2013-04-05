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
/// Description
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
            //TODO: Handle learning AI stuff
            handleLearningStart(wRightMallet, wPuck, wLeftMallet);
        }
        else if(wLeftPlayer->obtenirType() == JOUEUR_VIRTUEL_RENFORCEMENT && (mPreviousPuckPosition[VX] > 0 && wPuckPosition[VX] <= 0))
        {
            //TODO: Handle learning AI stuff.
            handleLearningStart(wLeftMallet, wPuck, wRightMallet);
        }
        mPreviousPuckPosition = wPuckPosition;
    }
    Partie::animer(pTime);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn PartieApprentissage::modifierJoueurDroit( SPJoueurAbstrait pPlayer )
///
/// Description
///
/// @param[in] SPJoueurAbstrait pPlayer
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
/// Description
///
/// @param[in] SPJoueurAbstrait pPlayer
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

void PartieApprentissage::handleGoalScored( SPJoueurAbstrait pLearningPlayer, SPJoueurAbstrait pOpponent)
{
    if(pLearningPlayer->obtenirType() == JOUEUR_VIRTUEL_RENFORCEMENT)
    {
        //TODO Make AILearner take a player object to save data
        //((JoueurVirtuelRenforcement*)pLearningPlayer.get())->
        AILearner::obtenirInstance()->terminerSauvegardeNouvelleInfo(AI_OUTPUT_ADVERSAIRE_BUT_COMPTE);
    }

    if(pOpponent->obtenirType() == JOUEUR_VIRTUEL_RENFORCEMENT)
    {
        AILearner::obtenirInstance()->terminerSauvegardeNouvelleInfo(AI_OUTPUT_BUT_COMPTE);
    }
    mGoalScored = true;
}


void PartieApprentissage::incrementerPointsJoueurGauche( bool pForceUpdate /*= false*/ )
{
    handleGoalScored(obtenirJoueurGauche(), obtenirJoueurDroit());
    Partie::incrementerPointsJoueurGauche();
}

void PartieApprentissage::incrementerPointsJoueurDroit( bool pForceUpdate /*= false*/ )
{
    handleGoalScored(obtenirJoueurDroit(), obtenirJoueurGauche());
    Partie::incrementerPointsJoueurGauche();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn PartieApprentissage::handleLearningStart( NoeudMaillet* pRightMallet, NoeudRondelle* pPuck, NoeudMaillet* pOpponentMallet, Vecteur3 wPuckPosition )
///
/// Description
///
/// @param[in] NoeudMaillet * pRightMallet
/// @param[in] NoeudRondelle * pPuck
/// @param[in] NoeudMaillet * pOpponentMallet
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PartieApprentissage::handleLearningStart( NoeudMaillet* pRightMallet, NoeudRondelle* pPuck, NoeudMaillet* pOpponentMallet)
{
    if(mGoalScored)
    {
        AILearner::obtenirInstance()->terminerSauvegardeNouvelleInfo(AI_OUTPUT_RIEN);
    }
    Vecteur3 wAiPosition(pRightMallet->getPosition()),
        wAiVelocity(pRightMallet->obtenirVelocite()),
        wPuckPosition(pPuck->getPosition()),
        wPuckVelocity(pPuck->obtenirVelocite()),
        wOpponentPosition(pOpponentMallet->getPosition());
    LearningAiAction wAction = (LearningAiAction) (rand() % AI_ACTION_NB);
    AILearner::obtenirInstance()->sauvegarderNouvelleInfo(wAiPosition, wAiVelocity, wPuckPosition, wPuckVelocity, wOpponentPosition, wAction);
}
