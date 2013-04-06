//////////////////////////////////////////////////////////////////////////////
/// @file AIMailletRenforcement.cpp
/// @author Mathieu Parent 
/// @date 2013-03-31
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "AIMailletRenforcement.h"
#include "NoeudRondelle.h"
#include "NoeudMaillet.h"
#include "Terrain.h"
#include "Partie.h"
#include "FacadeModele.h"
#include "AIStratOffensiveRenforcement.h"
#include "JoueurVirtuelRenforcement.h"
#include <memory>
#include "NoeudAbstrait.h"
#include <iostream>


////////////////////////////////////////////////////////////////////////
///
/// @fn  AIMailletRenforcement::AIMailletRenforcement( const JoueurVirtuel& jv )
///
/// Constructeur
///
/// @param[in] const JoueurVirtuel & jv : reference du joueurVirtuel ayant cet Intelligence Artificiel
///										la reference force cette instance a exister et donc non-Null
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
AIMailletRenforcement::AIMailletRenforcement(JoueurVirtuel* jv): AIMaillet(jv)
{
    mPuckWasOnOppenentSize = false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn AIMailletRenforcement::~AIMailletRenforcement()
///
/// Destructeur.
///
/// @return
///
////////////////////////////////////////////////////////////////////////
AIMailletRenforcement::~AIMailletRenforcement()
{
	
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void AIMailletRenforcement::evaluerStrategie( NoeudRondelle* rondelle, NoeudMaillet* maillet )
///
/// Évaluation de la stratégie à utiliser selon la position de la 
/// rondelle et le côté du maillet
///
/// @param[in] NoeudRondelle * rondelle : pointeur sur la rondelle
/// @param[in] NoeudMaillet * maillet : pointeur sur le maillet du jv
///
/// @return
///
////////////////////////////////////////////////////////////////////////
void AIMailletRenforcement::evaluerStrategie( NoeudMaillet* maillet )
{
    Partie* wGame = FacadeModele::getInstance()->obtenirPartieCourante();

    checkf(wGame);
    if(wGame && wGame->getField())
    {
        NoeudRondelle* wPuck = wGame->getField()->getPuck();

        if(maillet->estAGauche() && wPuck->getPosition()[VX] >= 0)
        {
            // DEF uniquement
            mPuckWasOnOppenentSize = true;
            changerStrat(DEFENSIVE);
            return;
        }
        else if(!maillet->estAGauche() && wPuck->getPosition()[VX] <= 0)
        {
            // DEF Uniquement
            mPuckWasOnOppenentSize = true;
            changerStrat(DEFENSIVE);
            return;
        }
        else if(mPuckWasOnOppenentSize)
        {
            // Analyse
            // On utilise la prediction pour trouver la position de la rondelle a x=75
            float wPosX = 30.0f;
            if(maillet->estAGauche())
            {
                wPosX *= -1.0f;
            }
            PuckProjection wPred = wGame->getPuckProjection(wPosX, 2000);

            if(wPred.time == -1)
            {
                changerStrat(OFFENSIVE); // Ne fait que repousser la rondelle
                return;
            }
            std::cout << "Projection reussie" << std::endl;
            // Get good strat
            // Get info for input
            JoueurVirtuelRenforcement* wJoueur = (JoueurVirtuelRenforcement*)jv_;
            Vecteur3 wPosRondelle = wGame->getField()->getPuck()->getPosition();
            Vecteur3 wVelRondelle = wGame->getField()->getPuck()->obtenirVelocite();
            Vecteur3 wPosAI = maillet->getPosition();
            Vecteur3 wVelAI = maillet->obtenirVelocite();
            Vecteur3 wPosAdversaire;
            if(maillet->estAGauche())
            {
                wPosAdversaire = wGame->obtenirJoueurDroit()->getControlingMallet()->getPosition();
            }
            else
            {
                wPosAdversaire = wGame->obtenirJoueurGauche()->getControlingMallet()->getPosition();
            }

            //LearningAiAction wAction = (LearningAiAction) (wJoueur->getActionFor(wPosAI, wVelAI, wPosRondelle, wVelRondelle, wPosAdversaire));
            LearningAiAction wAction = AI_ACTION_ATTAQUER_DIRECTEMENT;
            switch(wAction)
            {
            case AI_ACTION_DEFENDRE:
                changerStrat(DEFENSIVE);
                return; // Rien d'autre a set pour strat def
                break;
            case AI_ACTION_ATTAQUER_DIRECTEMENT:
                changerStrat(OFFENSIVE_LIGNE_DROITE);
                break;
            case AI_ACTION_ATTAQUER_DROITE:
                changerStrat(OFFENSIVE_DROITE);
                break;
            case AI_ACTION_ATTAQUER_GAUCHE:
                changerStrat(OFFENSIVE_GAUCHE);
                break;
            default:
                changerStrat(DEFENSIVE);
                return;
                break;
            }

            // Set important data
            AIStratOffensiveRenforcement* wStrat = (AIStratOffensiveRenforcement*) strategie_;
            wStrat->setPointImpact(wPred.position);
            wStrat->setTimeBeforeImpact(wPred.time);
            wStrat->calculateTagetPos();

            mPuckWasOnOppenentSize = false;
        }
        else
        {
            // Ne change rien, pas encore renvoye de l'autre cote
            return;
        }

        
    }
    else
    {
        // Si erreur, on donne une valeur arbitraire
        changerStrat(DEFENSIVE);
        return;
    }

}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



