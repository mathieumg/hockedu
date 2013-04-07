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

    SPJoueurVirtuel joueurVirtuel = std::dynamic_pointer_cast<JoueurVirtuel>(maillet->obtenirJoueur());
    checkf(joueurVirtuel);
	AIMaillet* aiMallet = joueurVirtuel->getAiMaillet();

	float sommeRayon = maillet->getRadius() + rondelle->getRadius() + 5; // minimum de 5 unités pour faire l'impulsion lors du coup
    

    if(!mCalculEffectue)
    {
        // Calcul pos [1]
        Vecteur3 wDir = (mMalletTargetPos-mPointVise);
        Vecteur3 wDirNorm = wDir;
        wDirNorm.normaliser();
        Vecteur3 wPoint1 = mPointImpact + 0.25f*wDir;
        Vecteur3 wDirRondelleNorm = rondelle->obtenirVelocite();
        wDirRondelleNorm.normaliser();

        Vecteur3 mMalletTargetPos = mPointImpact;//mPointImpact.convertir<2>() + (wDirRondelleNorm * (maillet->getRadius() + rondelle->getRadius()));


        AnimationFrame* frame[5];
        frame[0] = new AnimationFrame(0, maillet->getPosition());
        frame[1] = new AnimationFrame(mTimeBeforeImpact*0.75, wPoint1);
        frame[2] = new AnimationFrame(mTimeBeforeImpact*0.98, mMalletTargetPos);
        frame[3] = new AnimationFrame(mTimeBeforeImpact*1.01, mPointVise);
        frame[4] = new AnimationFrame(mTimeBeforeImpact*1.01, mPointVise);

        Animation* animation = new Animation(BEZIER, true, false, false);
        for(int i=0; i<3; i++)
            animation->ajouterFrame(frame[i]);
        animation->ajouterObjet((ObjetAnimable*)maillet);
        GestionnaireAnimations::obtenirInstance()->ajouterAnimation(animation);


        mCalculEffectue = true;
    }

    return Vecteur2();
}

