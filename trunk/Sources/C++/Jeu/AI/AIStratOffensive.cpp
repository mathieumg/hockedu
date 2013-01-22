//////////////////////////////////////////////////////////////////////////////
/// @file AIStratOffensive.cpp
/// @author Michael Ferris
/// @date 2012-04-02
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "AIStratOffensive.h"
#include "NoeudMaillet.h"
#include "Terrain.h"
#include "NoeudRondelle.h"
#include "AIMaillet.h"
#include "JoueurVirtuel.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn AIStratOffensive::AIStratOffensive()
///
/// Constructeur.
///
/// @param[in].
///
/// @return
///
////////////////////////////////////////////////////////////////////////
AIStratOffensive::AIStratOffensive(const AIMaillet& context):AIStrat(context)
{
	tirReussi_ = (unsigned int)(rand() % 100 + 1) > context_.obtenirJv().obtenirProbabiliteEchec();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn AIStratOffensive::~AIStratOffensive()
///
/// Destructeur.
///
/// @param[in].
///
/// @return
///
////////////////////////////////////////////////////////////////////////
AIStratOffensive::~AIStratOffensive()
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Vecteur2 AIStratOffensive::appliquerStrategie( NoeudMaillet* maillet, NoeudRondelle* rondelle )
///
/// On applique la strategie offensive, on retourne la direction a prendre du maillet
///
/// @param[in] NoeudMaillet * maillet : le maillet du JV
/// @param[in] NoeudRondelle * rondelle :  la rondelle
///
/// @return Vecteur2 :  la direction
///
////////////////////////////////////////////////////////////////////////
Vecteur2 AIStratOffensive::appliquerStrategie( NoeudMaillet* maillet )
{
	NoeudRondelle* rondelle; NoeudTable* table;
	if(!maillet->GetTerrain() || !( rondelle = maillet->GetTerrain()->getRondelle() ) || !( table = maillet->GetTerrain()->getTable() ) )
		return Vecteur2();

	// Direction du deplacement de la rondelle
	Vecteur2 dir = rondelle->obtenirPositionAbsolue().convertir<2>() - maillet->obtenirPositionAbsolue().convertir<2>();
	dir.normaliser();
	dir *= context_.obtenirJv().obtenirVitesse();

	if(!tirReussi_ && rondelle->obtenirVelocite().norme2() >= 800)
	{
		dir[VX] = 0;
	}

	
	return dir;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


