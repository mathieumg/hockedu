#include "AIStratOffensiveRenforcement.h"
#include "NoeudMaillet.h"
#include "Terrain.h"
#include "NoeudRondelle.h"
#include "AIMaillet.h"
#include "JoueurVirtuel.h"
#include "Renforcement\AIMailletRenforcement.h"

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
	tirReussi_ = (unsigned int)(rand() % 100 + 1) > context_.obtenirJv().obtenirProbabiliteEchec();
	malletTargetPos = Vecteur2();
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

	AIMaillet* aiMallet = ((JoueurVirtuel*)maillet->obtenirJoueur())->getAiMaillet();

	float sommeRayon = maillet->getRadius() + rondelle->getRadius() + 5; // minimum de 5 unités pour faire l'impulsion lors du coup

	//std::pair<Vecteur2,Vecteur2> lignePrevue = ((AIMailletRenforcement*)aiMallet)->getLignePrevue();
// 
// 	Vecteur2 velocitePuck = rondelle->obtenirVelocite();
	// Equation de la droite de la rondelle
// 	float m = (mPointImpact[VY] - lignePrevue.first[1])/(mPointImpact[VX]-lignePrevue.first[0]);
// 	float b = lignePrevue.first[1] - (m * lignePrevue.first[0]);
	// Test si le maillet est deja sur la ligne ou la rondelle va passer
	if (maillet->getPosition() == malletTargetPos)
	{
		// Le maillet est en place, on peut attaquer
		if (abs(rondelle->getPosition()[VX])>=mPointImpact[VX])
		{
			Vecteur2 dirToGo = mPointVise - mPointImpact;
			return dirToGo;
		} 
		else
		{
			return Vecteur2();
		}
		
	} 
	else
	{
		// Sinon on doit calculer la position sur la ligne ou le maillet doit etre

		// Droite sur laquelle on veut envoyer la rondelle
		Vecteur2 directionLignePrevue = mPointVise-mPointImpact;
		Vecteur2 directionLignePrevueNormaliser = directionLignePrevue.norme();
		// Point sur la droite ou on doit positionner le maillet avant de faire le coup
		Vecteur2 posPointSurLigne;
		// Projection de la position du maillet sur la droite quon veut
		posPointSurLigne[0] = directionLignePrevueNormaliser[0]*maillet->getPosition()[0];
		posPointSurLigne[1] = directionLignePrevueNormaliser[1]*maillet->getPosition()[1];
		Vecteur2 distMailletRondelleSurLigne = mPointImpact - posPointSurLigne;
		if ( distMailletRondelleSurLigne.norme() < sommeRayon)
		{
			// On doit ajuster posPointSurLigne pour ne pas etre sur la position de la rondelle
			float angle = atan(directionLignePrevue[VY]/directionLignePrevue[VX]); // rad
			float deltaX = sommeRayon * cos(angle);// rad
			float deltaY = sommeRayon * sin(angle);// rad
			posPointSurLigne[VY] += deltaY;
			// On ajuste selon de quel bord la vitesse de la rondelle va
			posPointSurLigne[VX] += (rondelle->obtenirVelocite()[VX] < 0 ? -deltaX : deltaX);

		} 

		// On sauvegarde la position quon veut etre sur la ligne
		malletTargetPos = posPointSurLigne;

		Vecteur2 dirToGo = posPointSurLigne-maillet->getPosition();
		return dirToGo;
	}
}

