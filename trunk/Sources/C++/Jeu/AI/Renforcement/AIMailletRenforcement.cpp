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
AIMailletRenforcement::AIMailletRenforcement(const JoueurVirtuel& jv): AIMaillet(jv)
{

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
/// @fn Vecteur2 AIMailletRenforcement::evaluerDirection( NoeudMaillet* maillet, NoeudTable* table,NoeudRondelle* rondelle )
///
/// On evalue la nouvelle direction du maillet contrôlé par joueur 
/// virtuel selon la strategie
///
/// @param[in] NoeudMaillet * maillet : pointeur sur le maillet du JV
/// @param[in] NoeudTable * table : pointeur sur la table pour savoir 
/// où sont les murets
///
/// @return Vecteur2
///
////////////////////////////////////////////////////////////////////////
Vecteur2 AIMailletRenforcement::evaluerDirection( NoeudMaillet* maillet )
{
	// A ce point, la methode evaluerStrategie a deja été appelé, donc on peut l'utiliser directement
	return Vecteur2();
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
	
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



