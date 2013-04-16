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
    changerStrat(DEFENSIVE);
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
    Terrain* wField = maillet->getField();
    Partie* wGame = NULL;

    if(wField)
    {
        wGame = wField->getGame();
    }

    if(wGame)
    {
        NoeudRondelle* wPuck = wField->getPuck();

        typeStrat wNextStrat = getNextStrat();
        if(wNextStrat != NBSTRAT)
        {
            changerStrat(wNextStrat);
            return;
        }

        if(maillet->estAGauche() && wPuck->getPosition()[VX] > -wPuck->getRadius()*2.0f)
        {
            changerStrat(DEFENSIVE);
            mPuckWasOnOppenentSize = true;
        }
        else if(!maillet->estAGauche() && wPuck->getPosition()[VX] < wPuck->getRadius()*2.0f)
        {
            changerStrat(DEFENSIVE);
            mPuckWasOnOppenentSize = true;
        }
        else
        {
            if(mPuckWasOnOppenentSize)
            {
                // Choix apprentissage
                // On utilise la prediction pour trouver la position de la rondelle a x=60 environ

                // Calcul d'une posX a frapper en fct de la vitesse de la rondelle
                NoeudRondelle* wPuck = wField->getPuck();

                float wVitRondelle = wPuck->obtenirVelocite().norme();
                float wPourVitesse = wVitRondelle / 1000.0f;


                float wPosX = 30.0f + 70.0f * wPourVitesse;
                if(maillet->estAGauche())
                {
                    wPosX *= -1.0f;
                }
                PuckProjection wPred = wGame->getPuckProjection(wPosX, 4000);

                if(wPred.time == -1)
                {
                    changerStrat(OFFENSIVE); // Ne fait que repousser la rondelle
                    return;
                }
// #if !SHIPPING
//                 std::cout << "Projection reussie " << wPred.position << std::endl;
// #endif //!SHIPPING
                // Get good strat
                // Get info for input
                JoueurVirtuelRenforcement* wJoueur = (JoueurVirtuelRenforcement*)jv_;
                Vecteur3 wPosRondelle = wPuck->getPosition();
                Vecteur3 wVelRondelle = wPuck->obtenirVelocite();
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
                
                LearningAiAction wAction = (LearningAiAction) (wJoueur->getActionFor(wPosAI, wVelAI, wPosRondelle, wVelRondelle, wPosAdversaire));
                // Tests:
                //LearningAiAction wAction = (LearningAiAction) (rand() % AI_ACTION_NB);
                switch(wAction)
                {
                case AI_ACTION_ATTAQUER_DIRECTEMENT:
#if !SHIPPING
                    if(!wJoueur->isLearning())
                        std::cout << "Action: Attaquer Directement" << std::endl;
#endif
                    changerStrat(OFFENSIVE_LIGNE_DROITE);
                    break;
                case AI_ACTION_ATTAQUER_DROITE:
#if !SHIPPING
                    if(!wJoueur->isLearning())
                        std::cout << "Action: Attaquer Droite" << std::endl;
#endif
                    changerStrat(OFFENSIVE_DROITE);
                    break;
                case AI_ACTION_ATTAQUER_GAUCHE:
#if !SHIPPING
                    if(!wJoueur->isLearning())
                        std::cout << "Action: Attaquer Gauche" << std::endl;
#endif
                    changerStrat(OFFENSIVE_GAUCHE);
                    break;
                case AI_ACTION_DEFENDRE:
                default:
#if !SHIPPING
                    if(!wJoueur->isLearning())
                        std::cout << "Action: Defendre (error)" << std::endl;
#endif
                    changerStrat(DEFENSIVE);
                    return; // Rien d'autre a set pour strat def
                    break;
                }

                // Set important data
                AIStratOffensiveRenforcement* wStrat = (AIStratOffensiveRenforcement*) strategie_;
                wStrat->setPointImpact(wPred.position);
                wStrat->setTimeBeforeImpact(wPred.time);
                wStrat->calculateTargetPos();
            }
            else if(maillet->estAGauche() && maillet->getPosition()[VX] > wPuck->getPosition()[VX] || !maillet->estAGauche() && maillet->getPosition()[VX] < wPuck->getPosition()[VX])
            {
                // Mode defensif qui contourne la rondelle (si derriere)
                changerStrat(DEFENSIVE);
            }
            mPuckWasOnOppenentSize = false;
        }
    }
    else
    {
        checkf(0);// Ne devrait pas arriver
        // Si erreur, on donne une valeur arbitraire
        changerStrat(DEFENSIVE);
        return;
    }

}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



