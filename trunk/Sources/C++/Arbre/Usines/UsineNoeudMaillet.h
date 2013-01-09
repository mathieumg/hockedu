///////////////////////////////////////////////////////////////////////////
/// @file UsineNoeudMaillet.h
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_USINES_USINENOEUDMAILLET_H__
#define __ARBRE_USINES_USINENOEUDMAILLET_H__


#include "UsineNoeud.h"
#include "NoeudMaillet.h"


///////////////////////////////////////////////////////////////////////////
/// @class UsineNoeudMaillet
/// @brief Classe qui repr�sente une usine capable de cr�er des noeuds de
///        type NoeudMaillet.
///
/// @author Mathieu Parent
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
class UsineNoeudMaillet : public UsineNoeud
{
public:
   /// Constructeur par param�tres.
   inline UsineNoeudMaillet(const std::string& nom);

   /// Fonction � surcharger pour la cr�ation d'un noeud.
   inline virtual NoeudAbstrait* creerNoeud() const;

   static bool bypassLimitePourTest; //D�finie dans NoeudMaillet.cpp
};


////////////////////////////////////////////////////////////////////////
///
/// @fn UsineNoeudMaillet::UsineNoeudMaillet(const std::string& nom)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par d�faut aux variables membres..
///
/// @param[in] nom   : Le nom de l'usine qui correspond au type de noeuds cr��s.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
inline UsineNoeudMaillet::UsineNoeudMaillet(const std::string& nom)
   : UsineNoeud(nom)
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAbstrait* UsineNoeudMaillet::creerNoeud() const
///
/// Cette fonction retourne un noeud nouvellement cr�� du type produit
/// par cette usine.
///
/// @return Le noeud nouvellement cr��.
///
////////////////////////////////////////////////////////////////////////
NoeudAbstrait* UsineNoeudMaillet::creerNoeud() const
{
	if(UsineNoeudMaillet::bypassLimitePourTest || NoeudMaillet::mailletExistant < 2)
	{
		return new NoeudMaillet(obtenirNom());
	}
	return 0;
}


#endif // __ARBRE_USINES_USINENOEUDMAILLET_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
