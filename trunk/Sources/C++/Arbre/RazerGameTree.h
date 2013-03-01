///////////////////////////////////////////////////////////////////////////
/// @file RazerGameTree.h
/// @author Martin Bisson
/// @date 2007-03-23
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_RAZERGAMETREE_H__
#define __ARBRE_RAZERGAMETREE_H__


#include "ArbreRendu.h"

class NoeudTable;

///////////////////////////////////////////////////////////////////////////
/// @class RazerGameTree
/// @brief Classe qui représente l'arbre de rendu spécifique au projet
///        
///
///        Cette classe s'occupe de configurer les usines des noeuds qui
///        seront utilisés par le projet.
///
/// @author Martin Bisson
/// @date 2007-03-23
///////////////////////////////////////////////////////////////////////////
class RazerGameTree : public ArbreRendu
{
public:
    typedef ArbreRendu Super;
   /// Constructeur par défaut.
   RazerGameTree(class Terrain* pField,unsigned int limitMallet = 2, unsigned int limitPuck = 1);
   /// Destructeur.
   virtual ~RazerGameTree();

   /// Retourne le noeud représentant la table elle-même
   NoeudTable* obtenirTable();
   /// Creation du noeud XML du Noeud
   virtual XmlElement* creerNoeudXML();
};



#endif // __ARBRE_RazerGameTree_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
