//////////////////////////////////////////////////////////////////////////////
/// @file RazerGameTypeDef.h
/// @author Michael Ferris
/// @date 2012-02-02
/// @version 2.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once


#include <vector>
#include <map>
#include <set>
#include <string>
#include <memory>

class PlayerAbstract;
class PlayerComputer;
class PlayerReinforcementAI;
class PlayerHuman;
class PlayerNetwork;
class PlayerNetworkServer;
class NoeudAbstrait;

/////////////////////////////////////////////////////////////////////////////
/// Ce fichier contient différents type def utilisées dans plusieurs classes.
/////////////////////////////////////////////////////////////////////////////
#if WIN32

typedef std::shared_ptr<PlayerAbstract> SPPlayerAbstract;
typedef std::shared_ptr<PlayerComputer> SPPlayerComputer;
typedef std::shared_ptr<PlayerReinforcementAI> SPPlayerReinforcementAI;
typedef std::shared_ptr<PlayerHuman> SPPlayerHuman;
typedef std::shared_ptr<PlayerNetwork> SPPlayerNetwork;
typedef std::shared_ptr<PlayerNetworkServer> SPPlayerNetworkServer;

#else

typedef PlayerAbstract* SPPlayerAbstract;
typedef PlayerComputer* SPPlayerComputer;
typedef PlayerReinforcementAI* SPPlayerReinforcementAI;
typedef PlayerHuman* SPPlayerHuman;
typedef PlayerNetwork* SPPlayerNetwork;
typedef PlayerNetworkServer* SPPlayerNetworkServer;

#endif


/// Pointeur Intelligent sur les Joueurs

/// Typedef pour le conteneur des joueurs participant à un tournoi
typedef std::vector<SPPlayerAbstract> JoueursParticipant;
/// Typedef pour le conteneurdes joueurs existant
typedef std::map<std::string,SPPlayerAbstract> ConteneurJoueur;
/// Typedef pour le conteneur des noms des joueurs triés en ordre alphabétique
typedef std::set<std::string> ConteneurJoueursTries;
/// Conteneur pour les Noeud.
typedef std::vector<NoeudAbstrait*> ConteneurNoeuds;
/// Structure contenant les ID d'un noeud pour la selection openGl  le premier est le ID et le 2e le typeID
typedef std::set<unsigned int> IdNoeuds;
/// Conteneur pour les IDs des noeuds pour la selection openGL
typedef std::map<unsigned int, IdNoeuds> ConteneurIdNoeuds;
/// Conteneur pour les Bonus des noeuds
typedef std::set<class BonusModifierAbstract*> ModifiersContainer;

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



