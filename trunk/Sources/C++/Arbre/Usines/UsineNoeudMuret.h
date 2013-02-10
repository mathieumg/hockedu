///////////////////////////////////////////////////////////////////////////
/// @file UsineNoeudMuret.h
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_USINES_USINENOEUDMURET_H__
#define __ARBRE_USINES_USINENOEUDMURET_H__


#include "UsineNoeud.h"
#include "NoeudMuret.h"


///////////////////////////////////////////////////////////////////////////
/// @class UsineNoeudMuret
/// @brief Classe qui représente une usine capable de créer des noeuds de
///        type NoeudMuret.
///
/// @author Mathieu Parent
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
class UsineNoeudMuret : public UsineNoeud
{
public:
   /// Constructeur par paramètres.
   inline UsineNoeudMuret(const std::string& nom);

   /// Fonction à surcharger pour la création d'un noeud.
   inline virtual NoeudAbstrait* creerNoeud() const;


};


////////////////////////////////////////////////////////////////////////
///
/// @fn UsineNoeudMuret::UsineNoeudMuret(const std::string& nom)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par défaut aux variables membres..
///
/// @param[in] nom   : Le nom de l'usine qui correspond au type de noeuds créés.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
inline UsineNoeudMuret::UsineNoeudMuret(const std::string& nom)
   : UsineNoeud(nom)
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAbstrait* UsineNoeudMuret::creerNoeud() const
///
/// Cette fonction retourne un noeud nouvellement créé du type produit
/// par cette usine.
///
/// @return Le noeud nouvellement créé.
///
////////////////////////////////////////////////////////////////////////
NoeudAbstrait* UsineNoeudMuret::creerNoeud() const
{
   return new NoeudMuret(obtenirNom());
}


#endif // __ARBRE_USINES_USINENoeudMuret_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
