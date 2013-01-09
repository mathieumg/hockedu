//////////////////////////////////////////////////////////////////////////////
/// @file INF2990TypeDef.h
/// @author Michael Ferris
/// @date 2012-02-02
/// @version 2.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once


#include <vector>
#include <map>
#include <set>
#include <string>
#include <memory>

class JoueurAbstrait;
class JoueurVirtuel;
class JoueurHumain;
class NoeudAbstrait;

/////////////////////////////////////////////////////////////////////////////
/// Ce fichier contient différents type def utilisées dans plusieurs classes.
/////////////////////////////////////////////////////////////////////////////

/// Pointeur Intelligent sur les Joueurs
typedef std::shared_ptr<JoueurAbstrait> SPJoueurAbstrait;
typedef std::shared_ptr<JoueurVirtuel> SPJoueurVirtuel;
typedef std::shared_ptr<JoueurHumain> SPJoueurHumain;
/// Typedef pour le conteneur des joueurs participant à un tournoi
typedef std::vector<SPJoueurAbstrait> JoueursParticipant;
/// Typedef pour le conteneurdes joueurs existant
typedef std::map<std::string,SPJoueurAbstrait> ConteneurJoueur;
/// Typedef pour le conteneur des noms des joueurs triés en ordre alphabétique
typedef std::set<std::string> ConteneurJoueursTries;
/// Conteneur pour les Noeud.
typedef std::vector<NoeudAbstrait*> ConteneurNoeuds;
/// Structure contenant les ID d'un noeud pour la selection openGl  le premier est le ID et le 2e le typeID
typedef std::set<unsigned int> IdNoeuds;
/// Conteneur pour les IDs des noeuds pour la selection openGL
typedef std::map<unsigned int, IdNoeuds> ConteneurIdNoeuds;

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



