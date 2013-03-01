///////////////////////////////////////////////////////////////////////////////
/// @file UsineNoeud.h
/// @author Martin Bisson
/// @date 2007-01-28
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "UsineNoeud.h"
#include "NoeudGroupe.h"
#include "NoeudAccelerateur.h"
#include "NoeudMaillet.h"
#include "NoeudMuret.h"
#include "NoeudPortail.h"
#include "NoeudRondelle.h"
#include "NoeudTable.h"
#include "NodeWallEdition.h"
#include "NodeControlPoint.h"
#include "NodePolygone.h"

DEFAULT_NODE_FACTORY_IMPLEMENTATION(NoeudGroupe);
DEFAULT_NODE_FACTORY_IMPLEMENTATION(NoeudAccelerateur);
DEFAULT_NODE_FACTORY_IMPLEMENTATION(NoeudMuret);
DEFAULT_NODE_FACTORY_IMPLEMENTATION(NoeudPortail);
DEFAULT_NODE_FACTORY_IMPLEMENTATION(NoeudTable);
DEFAULT_NODE_FACTORY_IMPLEMENTATION(NodeWallEdition);
DEFAULT_NODE_FACTORY_IMPLEMENTATION(NodeControlPoint);
DEFAULT_NODE_FACTORY_IMPLEMENTATION(NodePolygone);
NODE_FACTORY_WITH_LIMIT_IMPLEMENTATION(NoeudMaillet);
NODE_FACTORY_WITH_LIMIT_IMPLEMENTATION(NoeudRondelle);



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
