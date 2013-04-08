#include "AIStratOffensiveRenforcement.h"
#include "NoeudMaillet.h"
#include "Terrain.h"
#include "NoeudRondelle.h"
#include "AIMaillet.h"
#include "JoueurVirtuel.h"
#include "Renforcement\AIMailletRenforcement.h"
#include "Vecteur.h"
#include <iostream>
#include "AnimationFrame.h"
#include "Animation.h"
#include "GestionnaireAnimations.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  AIStratOffensiveRenforcement::AIStratOffensiveRenforcement( const AIMaillet& )
///
/// /*Description*/
///
/// @param[in] const AIMaillet &
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
AIStratOffensiveRenforcement::AIStratOffensiveRenforcement( const AIMaillet& context):AIStrat(context)
{
	tirReussi_ = (unsigned int)(rand() % 100 + 1) > context_.obtenirJv()->obtenirProbabiliteEchec();
	mMalletTargetPos = Vecteur2();
    mCalculEffectue = false;
    mAttackMode = false;
}
////////////////////////////////////////////////////////////////////////
///
/// @fn  AIStratRenforcement::~AIStratRenforcement()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
AIStratOffensiveRenforcement::~AIStratOffensiveRenforcement()
{

}
////////////////////////////////////////////////////////////////////////
///
/// @fn Vecteur2 AIStratOffensiveRenforcement::appliquerStrategie( NoeudMaillet* maillet , Vecteur2 pointVise /*= Vecteur2() */ )
///
/// /*Description*/
///
/// @param[in] NoeudMaillet * maillet
/// @param[in] Vecteur2 pointVise
///
/// @return Vecteur2
///
////////////////////////////////////////////////////////////////////////
Vecteur2 AIStratOffensiveRenforcement::appliquerStrategie( NoeudMaillet* maillet )
{
	NoeudRondelle* rondelle; NoeudTable* table;
	if(!maillet->getField() || !( rondelle = maillet->getField()->getPuck() ) || !( table = maillet->getField()->getTable() ) )
		return Vecteur2();

    if(mGameTime.Elapsed_Time_sec() > 3)
    {
        // Si apres 3 sec on est encore en defensive, on attaque
        const_cast<AIMaillet&>(context_).changerStratNext(OFFENSIVE, AIStrat::TesterPushPuckOnTheOtherSide); // Va changer au prochain tick
        return Vecteur2(); // On doit return car on vient d'etre delete
    }

    SPJoueurVirtuel joueurVirtuel = std::dynamic_pointer_cast<JoueurVirtuel>(maillet->obtenirJoueur());
    checkf(joueurVirtuel);
	AIMaillet* aiMallet = joueurVirtuel->getAiMaillet();

	float sommeRayon = maillet->getRadius() + rondelle->getRadius() + 5; // minimum de 5 unités pour faire l'impulsion lors du coup
    

    if(!mCalculEffectue)
    {
        // On calcule une position "random" quand le tir fail
        Vecteur2 wPointImpactModifie = mPointImpact;
        Vecteur3 wDir = (mMalletTargetPos-mPointVise);
        Vecteur3 wDirNorm = wDir;
        wDirNorm.normaliser();
        if(!tirReussi_)
        {
            // Induire une erreur dans le calcul
            float wRayonMaillet = maillet->getRadius();
             int wSens = rand() % 1;
            Vecteur2 wErreur = Vecteur2(wDirNorm[VY], -wDirNorm[VX])*(wSens == 0 ? (1) : (-1))*2.0f*wRayonMaillet;

            wPointImpactModifie += wErreur;
        }

       
        Vecteur3 wPoint1 = wPointImpactModifie + 0.40f*wDir;
        Vecteur3 wDirRondelleNorm = rondelle->obtenirVelocite();
        wDirRondelleNorm.normaliser();

        Vecteur3 mMalletTargetPos = wPointImpactModifie.convertir<2>() + (wDirRondelleNorm * (maillet->getRadius() + rondelle->getRadius()));

        Vecteur3 wAjustement = wPointImpactModifie-mMalletTargetPos;
        wAjustement.normaliser();

        AnimationFrame* frame[4];
        frame[0] = new AnimationFrame(0.0f, maillet->getPosition());
        frame[1] = new AnimationFrame(mTimeBeforeImpact*0.75f, wPoint1);
        frame[2] = new AnimationFrame(mTimeBeforeImpact*0.98f, mMalletTargetPos-wAjustement*maillet->getRadius());
        frame[3] = new AnimationFrame((float)mTimeBeforeImpact, mMalletTargetPos);

        Animation* animation = new Animation(BEZIER, true, false, false);
        for(int i=0; i<4; i++)
            animation->ajouterFrame(frame[i]);
        animation->ajouterObjet((ObjetAnimable*)maillet);
        GestionnaireAnimations::obtenirInstance()->ajouterAnimation(animation);


        /*

        Vecteur3 wDirRondelleNorm = rondelle->obtenirVelocite();
        wDirRondelleNorm.normaliser();

        Vecteur3 mMalletTargetPos = mPointImpact;//mPointImpact.convertir<2>() + (wDirRondelleNorm * (maillet->getRadius() + rondelle->getRadius()));

        Vecteur3 wDir = (mMalletTargetPos-mPointVise);
        Vecteur3 wDirNorm = wDir;
        wDirNorm.normaliser();
        Vecteur3 wPoint1 = mPointImpact + 0.40f*wDir;
        

        AnimationFrame* frame[5];
        frame[0] = new AnimationFrame(0, maillet->getPosition());
        frame[1] = new AnimationFrame(mTimeBeforeImpact*0.75f, wPoint1);
        frame[2] = new AnimationFrame(mTimeBeforeImpact*0.9f, mMalletTargetPos + 2.0f*wDirRondelleNorm*maillet->getRadius());
        frame[3] = new AnimationFrame(mTimeBeforeImpact, mMalletTargetPos - 10.0f*wDirRondelleNorm*maillet->getRadius());
        frame[4] = new AnimationFrame(mTimeBeforeImpact, mMalletTargetPos - 10.0f*wDirRondelleNorm*maillet->getRadius());

        Animation* animation = new Animation(BEZIER, true, false, false);
        for(int i=0; i<5; i++)
            animation->ajouterFrame(frame[i]);
        animation->ajouterObjet((ObjetAnimable*)maillet);
        GestionnaireAnimations::obtenirInstance()->ajouterAnimation(animation);

        */
        mCalculEffectue = true;
    }

    return Vecteur2();
}

