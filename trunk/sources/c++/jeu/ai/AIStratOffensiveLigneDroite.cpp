//////////////////////////////////////////////////////////////////////////////
/// @file AIStratOffensiveLigneDroite.cpp
/// @author Mathieu Parent
/// @date 2013-03-31
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "AIStratOffensiveLigneDroite.h"
#include "NoeudMaillet.h"
#include "Terrain.h"
#include "NoeudRondelle.h"
#include "AIMaillet.h"
#include "JoueurVirtuel.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn AIStratOffensiveLigneDroite::AIStratOffensiveLigneDroite()
///
/// Constructeur.
///
/// @param[in].
///
/// @return
///
////////////////////////////////////////////////////////////////////////
AIStratOffensiveLigneDroite::AIStratOffensiveLigneDroite(const AIMaillet& context):AIStrat(context)
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn AIStratOffensiveLigneDroite::~AIStratOffensiveLigneDroite()
///
/// Destructeur.
///
/// @param[in].
///
/// @return
///
////////////////////////////////////////////////////////////////////////
AIStratOffensiveLigneDroite::~AIStratOffensiveLigneDroite()
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Vecteur2 AIStratOffensiveLigneDroite::appliquerStrategie( NoeudMaillet* maillet, NoeudRondelle* rondelle )
///
/// On applique la strategie offensive, on retourne la direction a prendre du maillet
///
/// @param[in] NoeudMaillet * maillet : le maillet du JV
/// @param[in] NoeudRondelle * rondelle :  la rondelle
///
/// @return Vecteur2 :  la direction
///
////////////////////////////////////////////////////////////////////////
Vecteur2 AIStratOffensiveLigneDroite::appliquerStrategie( NoeudMaillet* maillet, Vecteur2 pointVise /*= Vecteur2()*/ )
{
	NoeudRondelle* rondelle; NoeudTable* table;
	if(!maillet->getField() || !( rondelle = maillet->getField()->getPuck() ) || !( table = maillet->getField()->getTable() ) )
		return Vecteur2();

    // On va chercher la position de la rondelle et du maillet
    Vecteur3 wPosRondelle   = rondelle->getPosition();
    Vecteur3 wPosMaillet    = maillet->getPosition();
    Vecteur3 wDistanceVecteur = wPosRondelle-wPosMaillet;
    float wDistance = sqrt(wDistanceVecteur[VX] * wDistanceVecteur[VX] + wDistanceVecteur[VY] * wDistanceVecteur[VY]);

    if(wDistance > 50.0f)
    {
        // Si on est loin, on va en direction de la rondelle
        wDistanceVecteur.normaliser();
        wDistanceVecteur *= (float)context_.obtenirJv().obtenirVitesse();
        return wDistanceVecteur;
    }
    else
    {
        // Si on est proche de la rondelle, on triche un peu pour que la trajectoire apres impact soit vers le but adverse directement
        NoeudBut* wButs[2];
        maillet->getField()->getGoals(wButs);
        if(!wButs[0] || !wButs[1])
        {
            return Vecteur2();
        }
        else
        {
            // On a les buts, on veut viser dans cette direction

            // TODO: IMPLEMENTATION DIRECTION

        }
    }
    return Vecteur2();
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


