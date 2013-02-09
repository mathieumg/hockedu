///////////////////////////////////////////////////////////////////////////
/// @file RazerGameTree.h
/// @author Martin Bisson
/// @date 2007-03-23
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_RAZERGAMETREE_H__
#define __ARBRE_RAZERGAMETREE_H__


#include "ArbreRendu.h"

class NoeudTable;

///////////////////////////////////////////////////////////////////////////
/// @class RazerGameTree
/// @brief Classe qui repr�sente l'arbre de rendu sp�cifique au projet
///        
///
///        Cette classe s'occupe de configurer les usines des noeuds qui
///        seront utilis�s par le projet.
///
/// @author Martin Bisson
/// @date 2007-03-23
///////////////////////////////////////////////////////////////////////////
class RazerGameTree : public ArbreRendu
{
public:
   /// Constructeur par d�faut.
   RazerGameTree();
   /// Destructeur.
   virtual ~RazerGameTree();

   /// Initialise l'arbre de rendu � son �tat initial.
   void initialiser();

   /// Retourne le noeud repr�sentant la table elle-m�me
   NoeudTable* obtenirTable();

};



#endif // __ARBRE_RazerGameTree_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
