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
Vecteur2 AIStratOffensiveRenforcement::appliquerStrategie( NoeudMaillet* maillet , Vecteur2 pointVise /*= Vecteur2() */ )
{
	NoeudRondelle* rondelle; NoeudTable* table;
	if(!maillet->getField() || !( rondelle = maillet->getField()->getPuck() ) || !( table = maillet->getField()->getTable() ) )
		return Vecteur2();

	AIMaillet* aiMallet = ((JoueurVirtuel*)maillet->obtenirJoueur())->getAiMaillet();
	std::pair<Vecteur2,Vecteur2> lignePrevue = ((AIMailletRenforcement*)aiMallet)->getLignePrevue();

	// Equation de la droite
	float m = (lignePrevue.second[1] - lignePrevue.first[1])/(lignePrevue.second[0]-lignePrevue.first[0]);
	float b = lignePrevue.first[1] - (m * lignePrevue.first[0]);
	// Test si le maillet est deja sur la ligne ou la rondelle va passer
	if (maillet->getPosition() == malletTargetPos)
	{
		// On est en place, on peut attaquer
		return Vecteur2();
	} 
	else
	{
		// Sinon on doit calculer la position sur la ligne ou le maillet doit etre
		Vecteur2 directionLignePrevue = lignePrevue.second-lignePrevue.first;
		Vecteur2 directionLignePrevueNormaliser = directionLignePrevue.norme();
		Vecteur2 posPointSurLigne;
		posPointSurLigne[0] = directionLignePrevueNormaliser[0]*maillet->getPosition()[0];
		posPointSurLigne[1] = directionLignePrevueNormaliser[1]*maillet->getPosition()[1];
		float sommeRayon = maillet->getRadius() + rondelle->getRadius() + 5; // minimum de 5 unités pour faire l'impulsion
		Vecteur2 distMailletRondelleSurLigne = (Vecteur2)lignePrevue.second - posPointSurLigne;
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
		return (dirToGo/=dirToGo.norme() * (float)context_.obtenirJv().obtenirVitesse());
	}
}

