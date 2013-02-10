///////////////////////////////////////////////////////////////////////////
/// @file UsineNoeudTable.h
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_USINES_USINENOEUDTABLE_H__
#define __ARBRE_USINES_USINENOEUDTABLE_H__


#include "UsineNoeud.h"
#include "NoeudTable.h"


///////////////////////////////////////////////////////////////////////////
/// @class UsineNoeudTable
/// @brief Classe qui représente une usine capable de créer des noeuds de
///        type NoeudTable.
///
/// @author Mathieu Parent
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
class UsineNoeudTable : public UsineNoeud
{
public:
   /// Constructeur par paramètres.
   inline UsineNoeudTable(const std::string& nom);

   /// Fonction à surcharger pour la création d'un noeud.
   inline virtual NoeudAbstrait* creerNoeud() const;


};


////////////////////////////////////////////////////////////////////////
///
/// @fn UsineNoeudTable::UsineNoeudTable(const std::string& nom)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par défaut aux variables membres..
///
/// @param[in] nom   : Le nom de l'usine qui correspond au type de noeuds créés.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
inline UsineNoeudTable::UsineNoeudTable(const std::string& nom)
   : UsineNoeud(nom)
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAbstrait* UsineNoeudTable::creerNoeud() const
///
/// Cette fonction retourne un noeud nouvellement créé du type produit
/// par cette usine
///
/// @return Le noeud nouvellement créé.
///
////////////////////////////////////////////////////////////////////////
NoeudAbstrait* UsineNoeudTable::creerNoeud() const
{
   return new NoeudTable(obtenirNom());
}


#endif // __ARBRE_USINES_USINENOEUDTABLE_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
