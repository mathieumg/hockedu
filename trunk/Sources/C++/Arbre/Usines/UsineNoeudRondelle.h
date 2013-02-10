///////////////////////////////////////////////////////////////////////////
/// @file UsineNoeudRondelle.h
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_USINES_USINENOEUDRONDELLE_H__
#define __ARBRE_USINES_USINENOEUDRONDELLE_H__


#include "UsineNoeud.h"
#include "NoeudRondelle.h"


///////////////////////////////////////////////////////////////////////////
/// @class UsineNoeudRondelle
/// @brief Classe qui repr�sente une usine capable de cr�er des noeuds de
///        type NoeudRondelle.
///
/// @author Mathieu Parent
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
class UsineNoeudRondelle : public UsineNoeud
{
public:
   /// Constructeur par param�tres.
   inline UsineNoeudRondelle(const std::string& nom);

   /// Fonction � surcharger pour la cr�ation d'un noeud.
   inline virtual NoeudAbstrait* creerNoeud() const;

   static bool bypassLimitePourTest; // D�finie dans NoeudRondelle.cpp

};


////////////////////////////////////////////////////////////////////////
///
/// @fn UsineNoeudRondelle::UsineNoeudRondelle(const std::string& nom)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par d�faut aux variables membres..
///
/// @param[in] nom   : Le nom de l'usine qui correspond au type de noeuds cr��s.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
inline UsineNoeudRondelle::UsineNoeudRondelle(const std::string& nom)
   : UsineNoeud(nom)
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAbstrait* UsineNoeudRondelle::creerNoeud() const
///
/// Cette fonction retourne un noeud nouvellement cr�� du type produit
/// par cette usine.
///
/// @return Le noeud nouvellement cr��.
///
////////////////////////////////////////////////////////////////////////
NoeudAbstrait* UsineNoeudRondelle::creerNoeud() const
{
	if(UsineNoeudRondelle::bypassLimitePourTest || NoeudRondelle::rondellesPresentes < 1)
		return new NoeudRondelle(obtenirNom());
	return 0;
}


#endif // __ARBRE_USINES_USINENOEUDRONDELLE_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
