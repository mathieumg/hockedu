///////////////////////////////////////////////////////////////////////////
/// @file UsineNoeudGroupe.h
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_USINES_UsineNoeudGroupe_H__
#define __ARBRE_USINES_UsineNoeudGroupe_H__


#include "UsineNoeud.h"
#include "NoeudGroupe.h"


///////////////////////////////////////////////////////////////////////////
/// @class UsineNoeudGroupe
/// @brief Classe qui représente une usine capable de créer des noeuds de
///        type NoeudMaillet.
///
/// @author Mathieu Parent
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
class UsineNoeudGroupe : public UsineNoeud
{
public:
	/// Constructeur par paramètres.
	inline UsineNoeudGroupe(const std::string& nom);

	/// Fonction à surcharger pour la création d'un noeud.
	inline virtual NoeudAbstrait* creerNoeud() const;

};


////////////////////////////////////////////////////////////////////////
///
/// @fn UsineNoeudGroupe::UsineNoeudGroupe(const std::string& nom)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par défaut aux variables membres..
///
/// @param[in] nom   : Le nom de l'usine qui correspond au type de noeuds créés.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
inline UsineNoeudGroupe::UsineNoeudGroupe(const std::string& nom)
	: UsineNoeud(nom)
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAbstrait* UsineNoeudGroupe::creerNoeud() const
///
/// Cette fonction retourne un noeud nouvellement créé du type produit
/// par cette usine.
///
/// @return Le noeud nouvellement créé.
///
////////////////////////////////////////////////////////////////////////
NoeudAbstrait* UsineNoeudGroupe::creerNoeud() const
{
	return new NoeudGroupe(obtenirNom());
}


#endif // __ARBRE_USINES_UsineNoeudGroupe_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
