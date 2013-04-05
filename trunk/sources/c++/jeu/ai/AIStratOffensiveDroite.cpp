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
AIStratOffensiveDroite::AIStratOffensiveDroite(const AIMaillet& context):AIStratOffensiveRenforcement(context)
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

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


