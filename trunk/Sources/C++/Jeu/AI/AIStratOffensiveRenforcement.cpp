#include "AIStratOffensiveRenforcement.h"
#include "NoeudMaillet.h"
#include "Terrain.h"
#include "NoeudRondelle.h"
#include "AIMaillet.h"
#include "JoueurVirtuel.h"
#include "Renforcement\AIMailletRenforcement.h"
#include "Vecteur.h"
#include <iostream>

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
    std::cout << "WTFDUDE" << std::endl;
	NoeudRondelle* rondelle; NoeudTable* table;
	if(!maillet->getField() || !( rondelle = maillet->getField()->getPuck() ) || !( table = maillet->getField()->getTable() ) )
		return Vecteur2();

    SPJoueurVirtuel joueurVirtuel = std::dynamic_pointer_cast<JoueurVirtuel>(maillet->obtenirJoueur());
    checkf(joueurVirtuel);
	AIMaillet* aiMallet = joueurVirtuel->getAiMaillet();

	float sommeRayon = maillet->getRadius() + rondelle->getRadius() + 5; // minimum de 5 unit�s pour faire l'impulsion lors du coup

	//std::pair<Vecteur2,Vecteur2> lignePrevue = ((AIMailletRenforcement*)aiMallet)->getLignePrevue();
// 
// 	Vecteur2 velocitePuck = rondelle->obtenirVelocite();
	// Equation de la droite de la rondelle
// 	float m = (mPointImpact[VY] - lignePrevue.first[1])/(mPointImpact[VX]-lignePrevue.first[0]);
// 	float b = lignePrevue.first[1] - (m * lignePrevue.first[0]);
	// Test si le maillet est deja sur la ligne ou la rondelle va passer
    Vecteur2 wDist = maillet->getPosition() - mMalletTargetPos;
	if (mAttackMode == true || abs(wDist.norme()) < 8.0f)
	{
        mAttackMode = true;
		// Le maillet est en place, on peut attaquer
		if (abs(rondelle->getPosition()[VX])>=mPointImpact[VX])
		{
			Vecteur2 dirToGo = mPointVise - maillet->getPosition();
            dirToGo.normaliser();
            dirToGo *= joueurVirtuel->obtenirVitesse();
            std::cout << "Dir1: " << dirToGo << std::endl;
			return dirToGo;
		} 
		else
		{
			return Vecteur2();
		}
	} 
	else
	{
        if(mCalculEffectue)
        {
            Vecteur2 dirToGo = mMalletTargetPos-maillet->getPosition();
            dirToGo.normaliser();
            dirToGo *= joueurVirtuel->obtenirVitesse();
            std::cout << "Dir2: " << dirToGo << std::endl;
            return dirToGo;
        }

		// Sinon on doit calculer la position sur la ligne ou le maillet doit etre

		// Droite sur laquelle on veut envoyer la rondelle
		Vecteur2 directionLignePrevue = mPointVise-mPointImpact;
		Vecteur2 directionLignePrevueNormaliser = directionLignePrevue;
		directionLignePrevueNormaliser.normaliser();
		// Point sur la droite ou on doit positionner le maillet avant de faire le coup
		Vecteur2 posPointSurLigne;
		// Projection de la position du maillet sur la droite quon veut
		//posPointSurLigne[0] = directionLignePrevueNormaliser[0]*maillet->getPosition()[0];
		//posPointSurLigne[1] = directionLignePrevueNormaliser[1]*maillet->getPosition()[1];
		Vecteur2 vecVersMaillet = maillet->getPosition() - mPointImpact;
		Vecteur2 vecVersVise = mPointVise-mPointImpact;
		vecVersVise.normaliser();
		float res = produitScalaire(vecVersMaillet,vecVersVise);
		posPointSurLigne =((vecVersVise)*res) + mPointImpact;



		Vecteur2 distMailletRondelleSurLigne = mPointImpact - posPointSurLigne;
		if ( distMailletRondelleSurLigne.norme() < sommeRayon)
		{
			// On doit ajuster posPointSurLigne pour ne pas etre sur la position de la rondelle
			/*float angle = atan(directionLignePrevue[VY]/directionLignePrevue[VX]); // rad
			float deltaX = sommeRayon * cos(angle);// rad
			float deltaY = sommeRayon * sin(angle);// rad
			posPointSurLigne[VY] += deltaY;
			// On ajuste selon de quel bord la vitesse de la rondelle va
			posPointSurLigne[VX] += (rondelle->obtenirVelocite()[VX] < 0 ? -deltaX : deltaX);*/

		} 

		// On sauvegarde la position quon veut etre sur la ligne
		mMalletTargetPos = posPointSurLigne;

		Vecteur2 dirToGo = mMalletTargetPos-maillet->getPosition();
        dirToGo.normaliser();
        dirToGo *= joueurVirtuel->obtenirVitesse();
        std::cout << "Dir3: " << dirToGo << std::endl;
        mCalculEffectue = true;
        return dirToGo;
	}
}

