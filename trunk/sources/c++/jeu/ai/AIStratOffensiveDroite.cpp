//////////////////////////////////////////////////////////////////////////////
/// @file AIStratOffensiveDroite.cpp
/// @author Mathieu Parent
/// @date 2013-03-31
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "AIStratOffensiveDroite.h"
#include "NoeudMaillet.h"
#include "Terrain.h"
#include "NoeudRondelle.h"
#include "AIMaillet.h"
#include "JoueurVirtuel.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn AIStratOffensiveDroite::AIStratOffensiveDroite()
///
/// Constructeur.
///
/// @param[in].
///
/// @return
///
////////////////////////////////////////////////////////////////////////
AIStratOffensiveDroite::AIStratOffensiveDroite(const AIMaillet& context):AIStrat(context)
{
	tirReussi_ = (unsigned int)(rand() % 100 + 1) > context_.obtenirJv().obtenirProbabiliteEchec();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn AIStratOffensiveDroite::~AIStratOffensiveDroite()
///
/// Destructeur.
///
/// @param[in].
///
/// @return
///
////////////////////////////////////////////////////////////////////////
AIStratOffensiveDroite::~AIStratOffensiveDroite()
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Vecteur2 AIStratOffensiveDroite::appliquerStrategie( NoeudMaillet* maillet, NoeudRondelle* rondelle )
///
/// On applique la strategie offensive, on retourne la direction a prendre du maillet
///
/// @param[in] NoeudMaillet * maillet : le maillet du JV
/// @param[in] NoeudRondelle * rondelle :  la rondelle
///
/// @return Vecteur2 :  la direction
///
////////////////////////////////////////////////////////////////////////
Vecteur2 AIStratOffensiveDroite::appliquerStrategie( NoeudMaillet* maillet, Vecteur2 pointVise /*= Vecteur2()*/ )
{
	NoeudRondelle* rondelle; NoeudTable* table;
	if(!maillet->getField() || !( rondelle = maillet->getField()->getPuck() ) || !( table = maillet->getField()->getTable() ) )
		return Vecteur2();

	// Direction du deplacement de la rondelle
	Vecteur2 dir = rondelle->getPosition().convertir<2>() - maillet->getPosition().convertir<2>();
	dir.normaliser();
	dir *= (float)context_.obtenirJv().obtenirVitesse();

	if(!tirReussi_ && rondelle->obtenirVelocite().norme2() >= 800)
	{
		dir[VX] = 0;
	}

	
	return dir;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


