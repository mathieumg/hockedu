//////////////////////////////////////////////////////////////////////////////
/// @file AIStratDefensive.cpp
/// @author Michael Ferris
/// @date 2012-04-02
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "AIStratDefensive.h"
#include "Terrain.h"
#include "NoeudTable.h"
#include "NoeudBut.h"
#include "NoeudMaillet.h"
#include "NoeudRondelle.h"
#include "AIMaillet.h"
#include "JoueurVirtuel.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn AIStratDefensive::AIStratDefensive()
///
/// Constructeur.
///
/// @param[in].
///
/// @return
///
////////////////////////////////////////////////////////////////////////
AIStratDefensive::AIStratDefensive(const AIMaillet& context):AIStrat(context)
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn AIStratDefensive::~AIStratDefensive()
///
/// Destructeur.
///
/// @return
///
////////////////////////////////////////////////////////////////////////
AIStratDefensive::~AIStratDefensive()
{
}
////////////////////////////////////////////////////////////////////////
///
/// @fn Vecteur2 AIStratDefensive::appliquerStrategie( NoeudMaillet* maillet, NoeudRondelle* rondelle )
///
/// On applique la strategie defensive, on retourne la direction a prendre du maillet
///
/// @param[in] NoeudMaillet * maillet : le maillet du JV
/// @param[in] NoeudRondelle * rondelle :  la rondelle
///
/// @return Vecteur2 :  la direction
///
////////////////////////////////////////////////////////////////////////
Vecteur2 AIStratDefensive::appliquerStrategie( NoeudMaillet* maillet )
{
	NoeudRondelle* rondelle;
	if(!maillet->GetTerrain() || !( rondelle = maillet->GetTerrain()->getRondelle() ) )
		return Vecteur2();

	// On obtient le but du JV
	NoeudBut* butJV;
	// joueur gauche
	int i = maillet->estAGauche() ? 1 : 2;
	butJV =  maillet->GetTerrain()->getTable()->obtenirBut(i);

	// On determine la direction a prendre entre le but et le maillet
	Vecteur2 dir= butJV->obtenirPositionAbsolue() - maillet->obtenirPositionAbsolue();

	// On senligne avec la rondelle si on est deja dans la zone de but
	if (abs(dir[VX]) < maillet->obtenirRayon()*2+10)
	{
		dir = rondelle->obtenirPositionAbsolue() - maillet->obtenirPositionAbsolue();
		dir.normaliser();
		dir[VX] = 0.0;
	}
	else
		dir.normaliser();

	dir *= (float)context_.obtenirJv().obtenirVitesse()*0.75f;

	return dir;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


