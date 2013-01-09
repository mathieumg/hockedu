///////////////////////////////////////////////////////////////////////////
/// @file UsineNoeudPortail.h
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_USINES_USINENOEUDPORTAIL_H__
#define __ARBRE_USINES_USINENOEUDPORTAIL_H__


#include "UsineNoeud.h"
#include "NoeudPortail.h"


///////////////////////////////////////////////////////////////////////////
/// @class UsineNoeudPortail
/// @brief Classe qui repr�sente une usine capable de cr�er des noeuds de
///        type NoeudPortail.
///
/// @author Mathieu Parent
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
class UsineNoeudPortail : public UsineNoeud
{
public:
   /// Constructeur par param�tres.
   inline UsineNoeudPortail(const std::string& nom);

   /// Fonction � surcharger pour la cr�ation d'un noeud.
   inline virtual NoeudAbstrait* creerNoeud() const;


};


////////////////////////////////////////////////////////////////////////
///
/// @fn UsineNoeudPortail::UsineNoeudPortail(const std::string& nom)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par d�faut aux variables membres..
///
/// @param[in] nom   : Le nom de l'usine qui correspond au type de noeuds cr��s.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
inline UsineNoeudPortail::UsineNoeudPortail(const std::string& nom)
   : UsineNoeud(nom)
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAbstrait* UsineNoeudPortail::creerNoeud() const
///
/// Cette fonction retourne un noeud nouvellement cr�� du type produit
/// par cette usine.
///
/// @return Le noeud nouvellement cr��.
///
////////////////////////////////////////////////////////////////////////
NoeudAbstrait* UsineNoeudPortail::creerNoeud() const
{
   return new NoeudPortail(obtenirNom());
}


#endif // __ARBRE_USINES_USINENOEUDPORTAIL_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
