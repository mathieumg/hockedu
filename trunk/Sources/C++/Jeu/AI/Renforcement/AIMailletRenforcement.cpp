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
/// On evalue la nouvelle direction du maillet contr�l� par joueur 
/// virtuel selon la strategie
///
/// @param[in] NoeudMaillet * maillet : pointeur sur le maillet du JV
/// @param[in] NoeudTable * table : pointeur sur la table pour savoir 
/// o� sont les murets
///
/// @return Vecteur2
///
////////////////////////////////////////////////////////////////////////
Vecteur2 AIMailletRenforcement::evaluerDirection( NoeudMaillet* maillet )
{
	// A ce point, la methode evaluerStrategie a deja �t� appel�, donc on peut l'utiliser directement
	return Vecteur2();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void AIMailletRenforcement::evaluerStrategie( NoeudRondelle* rondelle, NoeudMaillet* maillet )
///
/// �valuation de la strat�gie � utiliser selon la position de la 
/// rondelle et le c�t� du maillet
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



